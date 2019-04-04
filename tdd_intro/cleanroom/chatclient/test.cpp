#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;
/*
Implement chat application, that communicates via TCP sockets.
 There is list of requirenments to this application:
 * It receives user nickname through commandline arguments on start
 * It runs only for two clients
 * On start it checks if port 4444 is bound
    * if it is - connects to other client
    * if not - binds a port and waits for inbound connection
        * message "No one is here..." is displayed in UI
 * After establishing connection - handshake is performed:
    * client writes message with it's nickname and ':HELLO!' string to socket ("client:HELLO!")
    * server responses with it's nickname and ':HELLO!' magic ("server:HELLO!")
        * if server receives malformated message - it drops connection with this client
        * if client receives malformated message - it drops connection and exits
            * Error message is shown
    * Listening socket must be closed
        (i) More than one client is not allowed to connect

 * It must send messages via created connection, until it is dropped
    * Application outputs "me: " prefix for user input
    * User inputs message through console interface and presses ENTER
    * Text message is sent to socket
 * It must receive messages via created connection, until it is dropped
    * Message received through socket
    * Line with message and '@friend_name: ' prefix is displayed ("metizik: Hello!")
 * END of message is determined by '\0' byte
 * When connection is dropped by one of users
    * "You are alone now" message is displayed to user
    * Application exits

  (i) Client starts dialog

 OPTIONAL requirement:
    * If user enters '!exit!' message, application must close connection and exit
    * If user runs app with 'me' nickname - error with text "Username me is reserved and can not be used" is displayed and application exits
*/

class IConnection{
public:
    virtual ~IConnection(){};
    virtual std::string read() = 0;
    virtual void write(const std::string& message) = 0;
};

class MockConnection : public IConnection
{
public:
    MOCK_METHOD1(write, void(const std::string&));
    MOCK_METHOD0(read, std::string());
};

const std::string kHandshakeSuffix = ":HELLO!";

void SendNickName(IConnection* connection, const std::string& nickname)
{
    connection->write(nickname + kHandshakeSuffix);
}

std::string ReceiveNickName(IConnection* connection)
{
    std::string message = connection->read();

    auto const expectedHandshakeSuffixPos = message.length() - kHandshakeSuffix.length();
    auto handshakeSuffixPos = message.find(kHandshakeSuffix);

    if (handshakeSuffixPos != expectedHandshakeSuffixPos ||
            handshakeSuffixPos == 0)
    {
        throw std::exception("Wrong handshake message.");
    }

    return message.substr(0, expectedHandshakeSuffixPos);
}

std::string ClientHandshake(IConnection* connection, const std::string &nickName)
{
    SendNickName(connection, nickName);
    return ReceiveNickName(connection);
}

std::string ServerHandshake(IConnection* connection, const std::string &nickName)
{
    std::string name = ReceiveNickName(connection);
    SendNickName(connection, nickName);

    return name;
}


TEST (SendNickname, send_nickname_to_iconnection)
{
    MockConnection connect;
    EXPECT_CALL(connect, write("NickName:HELLO!")).Times(1);

    SendNickName(&connect, "NickName");
}

TEST (ReceiveNickname, receive_nickname_from_iconnection)
{
    MockConnection connect;
    EXPECT_CALL(connect, read()).WillOnce(Return("Nickname:HELLO!"));

    EXPECT_EQ("Nickname", ReceiveNickName(&connect));
}

TEST (ReceiveNickname, receive_nickname_without_suffix_fails)
{
    MockConnection connect;
    EXPECT_CALL(connect, read()).WillOnce(Return("Nickname"));
    EXPECT_THROW(ReceiveNickName(&connect), std::exception);
}

TEST (ReceiveNickname, receive_nickname_with_suffix_in_wrong_place_fails)
{
    MockConnection connect;
    EXPECT_CALL(connect, read()).WillOnce(Return(":HELLO!Nickname"));
    EXPECT_THROW(ReceiveNickName(&connect), std::exception);
}

TEST (ReceiveNickname, receive_nickname_empty_nick)
{
    MockConnection connect;

    EXPECT_CALL(connect, read()).WillOnce(Return(":HELLO!"));
    EXPECT_THROW(ReceiveNickName(&connect), std::exception);
}

TEST(ClientHandshake, called_write_then_called_read)
{
    MockConnection connect;
    InSequence is;
    EXPECT_CALL(connect, write("nick:HELLO!")).WillOnce(Return());
    EXPECT_CALL(connect, read()).WillOnce(Return("bob:HELLO!"));

    EXPECT_EQ("bob", ClientHandshake(&connect, "nick"));
}

TEST(ServerHandshake, called_read_then_called_write)
{
    MockConnection connect;
    InSequence is;
    EXPECT_CALL(connect, read()).WillOnce(Return("bob:HELLO!"));
    EXPECT_CALL(connect, write("nick:HELLO!")).WillOnce(Return());

    EXPECT_EQ("bob", ServerHandshake(&connect, "nick"));
}
