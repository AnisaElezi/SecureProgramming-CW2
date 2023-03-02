#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/SMTPClientSession.h>

#include <iostream>
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
#include <mariadb/conncpp.hpp>
#include <cgicc/HTTPRedirectHeader.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <CkCrypt2.h>
#include <openssl/sha.h>
#include <iomanip>

using namespace cgicc;
using namespace std;

using namespace Poco::Net;
using namespace Poco;

Cgicc cgi;

//Applicant Dashbord
void ApplicantDashboard(string message = "") {
    try {

        cout << HTTPHTMLHeader() << endl;
        cout << html() << head(title("Brookes Job Portal")) << endl;
          cout << body();
        cout << "<div style=\"width: 100%; font-size: 36px; font-weight: bold; text-align: center; color: green;\">\n" <<endl;
        cout << p();
        cout << "<h2> Brookes Job Portal </h2> \n" <<endl; 
        cout << p();
        cout << "Confirm EmailApplicant" << endl;
        cout << "</div>\n" << endl;
      

        cout << form().set("method", "post") << endl;
        cout << "<div style=\"width:100%; text-align: center; display: inline-block;\">" << endl;
        cout << p("Em@iled OTP Code").set("class", "smalllable") << endl;
        cout << input().set("type", "password").set("name", "2FA").set("class", "input") << endl;
        cout << "<br>" << endl;
        if (!message.empty()) 
        {
            cout << p(message).set("class", "error") << endl;
            cout << "<br>" << endl;
        }
        cout << input().set("type", "submit").set("value", "Confirm").set("class", "button") << endl;
        cout << "<br>" << endl;
        cout << form() << endl;
        cout << body() << endl;
        cout << html();
    }
    catch (exception& e) {
        cout << "This is an error!. Please try to reload again." << endl; 
    }
}


//Company Dashbord
void CompanyDashboard(string message = "") {
    try {

        cout << HTTPHTMLHeader() << endl;
        cout << html() << head(title("Brookes Job Portal")) << endl;
          cout << body();
        cout << "<div style=\"width: 100%; font-size: 36px; font-weight: bold; text-align: center; color: green;\">\n" <<endl;
        cout << p();
        cout << "<h2> Brookes Job Portal </h2> \n" <<endl; 
        cout << p();
        cout << "Confirm Email by Company" << endl;
        cout << "</div>\n" << endl;
      

        cout << form().set("method", "post") << endl;
        cout << p("OTP Code send by E-Mail").set("class", "smalllable") << endl;
        cout << input().set("type", "password").set("name", "2FA").set("class", "input") << endl;
        cout << "<br>" << endl;
        if (!message.empty()) 
        {
            cout << p(message).set("class", "error") << endl;
            cout << "<br>" << endl;
        }
        cout << input().set("type", "submit").set("value", "Confirm").set("class", "button") << endl;
        cout << "<br>" << endl;
        cout << form() << endl;
        cout << body() << endl;
        cout << html();
    }
    catch (exception& e) {
        cout << "Error! Please try again." << endl; 
    }
}

/* admin Dashboard
    Prints the confirm registration page for the admin
*/
void AdminDashboard(string message = "", string secret = "") {
    try {

        cout << HTTPHTMLHeader() << endl;
        cout << html() << head(title("Brookes Job Portal")) << endl;
        cout << head() << endl;
        cout << body();
        
        cout << form().set("method", "post") << endl;
        cout << p("OTP Code send by E-Mail").set("class", "smalllable") << endl;
        cout << input().set("type", "password").set("name", "2FA").set("class", "input") << endl;
        cout << "<br>" << endl;
        cout << p("OTP Code ").set("class", "smalllable") << endl;
        cout << input().set("type", "password").set("name", "CR").set("class", "input") << endl;
        cout << "<br>" << endl;
        if (!message.empty()) {
            cout << p(message).set("class", "error") << endl;
            cout << "<br>" << endl;
        }
        cout << input().set("type", "submit").set("value", "Confirm").set("class", "button") << endl;
        cout << "<br>" << endl;
        cout << form() << endl;
        cout << p("Scann the code").set("class","smalllable") << endl;
        cout << a().set("id", "urilink") << endl;
        cout << cgicc::div().set("id", "qrcode") << endl;
        cout << img().set("id", "preview").set("src", "img/freeotp.svg") << endl;
        cout << cgicc::div() << endl;
        cout << a() << endl;

        cout << script() << endl;
        cout << "var qrcode = new QRCode(document.getElementById('qrcode'), { correctLevel : QRCode.CorrectLevel.H, text : window.location.href, colorLight : '#ffffff', colorDark : '#000000' }); " << endl;
        cout << "MakeQRCode('"+secret+"');" << endl;
        cout << script() << endl;

        cout << body() << endl;

        cout << html();
    }
    catch (exception& e) {
        cout << "This should not have happend. Please try to reload the webpage." << endl; // handle if applicable
    }
}

/* 
    saveSQLinjection() using whitelisting to ensure that input do not have any SQL injection
    allow alphanumerical and . _ @ characters in the string
*/
string saveSQLinjection(string input) {
    for (string::iterator i = input.begin(); i != input.end(); i++)
    {
        char c = input.at(i - input.begin());
        if (!(isalnum(c) || c == '@' || c == '.' || c == '_'))
        {
            input.erase(i);
            i--;
        }
    }
    return input;
}


// set one Cookie with the Username
void setCookie(string username) {
    cout << "Set-Cookie:username=";
    cout << username;
    cout << "; Domain=sots.brookes.ac.uk; ";
    cout << "Path=/; ";
    cout << "HTTPOnly=true; ";
    cout << "SameSite=Strict; ";
    cout << "Max-Age=300;\n";
}


// Redirection the user in case of any error, suspicious behavior.  
void error(string error="Please input valid data!") {
    cout << HTTPRedirectHeader("/register?error="+error) << endl;
    exit(0);
}

//Send Email to a given email
void sendEmail(string name, string email, string Code) {
    MailMessage msg;
    msg.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, email, name));
    msg.setSender("BrookeJobPortal <noreply@brookesjobportal.com>");
    msg.setSubject("OTP Code");
    msg.setContent(Code);
    SMTPClientSession smtp("smtp-relay.sendinblue.com", 587);
    smtp.login(Poco::Net::SMTPClientSession::LoginMethod::AUTH_LOGIN, "19177366@brookes.ac.uk", "hagtedeb-bsdhuwef7bshgsdh87239nsh8uhiw903jGyGJL9837uhjj0qwj7sJ70Lbs4212nd9WQm751hTnisnsdhc");
    smtp.sendMessage(msg);
    smtp.close();
}


    // Creates a OTP
bool create2FAToken(string username, string email) {
    time_t     ten = time(0) + 600;
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&ten);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    srand(time(0));
    int random = rand() % 10000;
    try {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect("ssh://sots.brookes.ac.uk/∼u19177366", "register", "bjagsdubesfhew762avTG5deW");
        con->setSchema("brookesjobportal");

        stmt = con->createStatement();
        string select = "INSERT INTO 2FA(code,fk_user,expires) VALUES(\"" + to_string(random) + "\",\"" + username + "\",\"" + buf + "\")";
        stmt->execute(select);

        sendEmail(username, email, to_string(random));
        return true;
    }
    catch (sql::SQLException& e) {
        return false;
    }
}


//converts a int to base32
string intToB32(long i)
{
    string b32 = "";
        int d = i % 32;
        if (d < 26)
        {
            b32.append(1,char((int('a')+d)));
        }
        else
        {
            b32.append(1, char(d-26+1+49));
        }

    return b32;
}

//Create a random seed through  intToB32() 
string setTOTP(string username) {
    username = saveSQLinjection(username);
    string secret;
    srand(time(0));
    for (int i = 0; i < 56; i++) {
        int random = rand() % 100;
        secret.append(intToB32(random));
    }

    try {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect("ssh://sots.brookes.ac.uk/∼u19177366/", "register", "bjagsdubesfhew762avTG5deW");
        con->setSchema("brookesjobportal");

        stmt = con->createStatement();
        string select = "INSERT INTO TOTP(seed,fk_user,active) VALUES('"+secret+"','" + username + "','false')";
        stmt->execute(select);

    }
    catch (sql::SQLException& e) {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect("ssh://sots.brookes.ac.uk/∼u19177366", "register", "bjagsdubesfhew762avTG5deW");
        con->setSchema("brookesjobportal");

        stmt = con->createStatement();
        string select = "UPDATE TOTP SET seed='" + secret + "' WHERE fk_user='"+username+"' AND active='0'";
        stmt->execute(select);
    }

    return secret;
}


 
//hashingSHA256() generates a hash from the input and returns it

string hashingSHA256(string str)
{
    unsigned char hash[hashingSHA256_DIGEST_LENGTH];
    hashingSHA256_CTX hashingSHA256;
    hashingSHA256_Init(&hashingSHA256);
    hashingSHA256_Update(&hashingSHA256, str.c_str(), str.size());
    hashingSHA256_Final(hash, &hashingSHA256);
    std::stringstream ss;
    for (int i = 0; i < hashingSHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}


/* 
    Saves the username, password, email and fullname if input valid
    Then send the OTP code for email verification and shows the TOTP QR Code of the admin is a user
*/
void completeRegistration(string username, string password, string email, string fullname) {

    username = saveSQLinjection(username);
    password = hashingSHA256(password);
    email = saveSQLinjection(email);
    fullname = saveSQLinjection(fullname);

    try {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect("ssh://sots.brookes.ac.uk/∼u19177366", "register", "bjagsdubesfhew762avTG5deW");
        con->setSchema("brookesjobportal");

        if (username == "admin") {
            bool active = true;
            stmt = con->createStatement();
            string select = "SELECT active FROM login WHERE name='admin';";
            res = stmt->executeQuery(select);
            if (res->next()) {
                active = res->getBoolean("active");
            }
            else {
                delete res;
                delete stmt;
                delete con;
                error("User already exist");
            }

            if (!active) {
                //add admin info
                stmt = con->createStatement();
                select = "UPDATE login SET active='0',password='"+password+"',email='"+email+"',fullname='"+fullname+"' WHERE name='admin';";
                stmt->execute(select);
                create2FAToken(username, email);
                setCookie(username);
                AdminDashboard("", setTOTP(username));
                exit(0);
            }
        }

        else if (username == "company") {
            bool active = true;
            stmt = con->createStatement();
            string select = "SELECT active FROM login WHERE name='company';";
            res = stmt->executeQuery(select);
            if (res->next()) {
                active = res->getBoolean("active");
            }
            else {
                delete res;
                delete stmt;
                delete con;
                error("User already exist");
            }

            if (!active) {
                //add company info
                stmt = con->createStatement();
                select = "UPDATE login SET active='0',password='"+password+"',email='"+email+"',fullname='"+fullname+"' WHERE name='company';";
                stmt->execute(select);
                create2FAToken(username, email);
                setCookie(username);
                CompanyDashboard("", setTOTP(username));
                exit(0);
            }
        }

           else  {
            bool active = true;
            stmt = con->createStatement();
            string select = "SELECT active FROM login WHERE name='applicant';";
            res = stmt->executeQuery(select);
            if (res->next()) {
                active = res->getBoolean("active");
            }
            else {
                delete res;
                delete stmt;
                delete con;
                error("User already exist");
            }

            if (!active) {
                //add admin info
                stmt = con->createStatement();
                select = "UPDATE login SET active='0',password='"+password+"',email='"+email+"',fullname='"+fullname+"' WHERE name='applicant';";
                stmt->execute(select);
                create2FAToken(username, email);
                setCookie(username);
                ApplicantDashboard("", setTOTP(username));
                exit(0);
            }
        }
    }
    catch (sql::SQLException& e) {
        error();
    }
    error();
}


void Confirmregistration(string username, string returnemail, string returntotp = "") {
    try {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect("ssh://sots,brookes.ac.uk/∼u19177366", "register", "bjagsdubesfhew762avTG5deW");
        con->setSchema("brookesjobportal");

        if (username == "admin") {
            bool active = true;
            stmt = con->createStatement();
            string select = "SELECT active FROM login WHERE name='admin'";
            res = stmt->executeQuery(select);
            if (res->next()) {
                active = res->getBoolean("active");
            }
            if (!active) {
                if (check_TOTP_Return(username, returntotp) && check_Email_Return(username, returnemail)) {
                    // User.active = true
                    stmt = con->createStatement();
                    select = "UPDATE login SET active='1' WHERE name='admin'";
                    stmt->execute(select);
                    // TOPT.active = true
                    stmt = con->createStatement();
                    select = "UPDATE totp SET active='1' WHERE fk_user='admin' AND active='0'";
                    stmt->execute(select);
                    cout << HTTPRedirectHeader("/?error=Registration successful") << endl;
                    exit(0);
                }
            }
            else {
                error("User already exists!");
            }
        }
        else {
            bool active = true;
            stmt = con->createStatement();
            string select = "SELECT active FROM login WHERE name='"+saveSQLinjection(username)+"'";
            res = stmt->executeQuery(select);
            if (res->next()) {
                active = res->getBoolean("active");
            }
            if (!active) {
                if (check_Email_Return(username, returnemail)) {
                    // User.active = true
                    stmt = con->createStatement();
                    select = "UPDATE login SET active='1' WHERE name='" + saveSQLinjection(username) + "'";
                    stmt->execute(select);
                    cout << HTTPRedirectHeader("/?error=Registration successful") << endl;
                    exit(0);
                }
            }
        }
    }
    catch (sql::SQLException& e) {
        error();
    }
    error();
    cout << HTTPRedirectHeader("/register/confirm.cgi") << endl;
    exit(0);
}


int main()
{
    string username = "";
    string password = "";
    string email = "";
    string fullname = "";
    string returnemail = "";
    string returntotp = "";

    form_iterator fi = cgi.getElement("username");
    
    if (!fi->isEmpty() && fi != (*cgi).end()) {
        username = **fi;

        fi = cgi.getElement("password");
        if (!fi->isEmpty() && fi != (*cgi).end())
            password = **fi;

        fi = cgi.getElement("email");
        if (!fi->isEmpty() && fi != (*cgi).end())
            email = **fi;

        fi = cgi.getElement("fullname");
        if (!fi->isEmpty() && fi != (*cgi).end())
            fullname = **fi;

        completeRegistration(username, password, email, fullname);

    }
    else {

        const_cookie_iterator cci;
        const CgiEnvironment& env = cgi.getEnvironment();

        for (cci = env.getCookieList().begin(); cci != env.getCookieList().end(); ++cci) {
            if (cci->getName() == "username") {
                username = cci->getValue();
            }
        }
        if (!username.empty()) {

            fi = cgi.getElement("2FA");
            if (!fi->isEmpty() && fi != (*cgi).end()) {
                returnemail = **fi;
            }
            fi = cgi.getElement("CR");
            if (!fi->isEmpty() && fi != (*cgi).end()) {
                returntotp = **fi;
            }
            if (username == "admin" && (returnemail.empty() || returntotp.empty())) {
                AdminDashboard("authentication error");
                exit(0);
            }
            else if (returnemail.empty() && username != "admin") {
                ApplicantDashboard("authentication error");
                exit(0);
            }
            else if (username == "admin" && !returnemail.empty() && !returntotp.empty()) {
                Confirmregistration(username, returnemail, returntotp);
            }
            else if (!returnemail.empty() && username != "admin" && returntotp.empty()) {
                Confirmregistration(username, returnemail);
            }
        }
        else {
            cout << HTTPRedirectHeader("/register/?error=No registration details provided") << endl;
            exit(0);
        }

    }
}

