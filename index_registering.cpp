
#include <iostream>
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
#include <mariadb/conncpp.hpp>

using namespace cgicc;
using namespace std;

Cgicc cgi;

//Prints the HTML code of the registration index page

void firstPage(string message = "") {
    try {

        cout << HTTPHTMLHeader() << endl;
        cout << html() << head(title("Brookes Job Portal")) << endl;
        cout << body();
        cout << "<div style=\"width: 100%; font-size: 36px; font-weight: bold; text-align: center; color: green;\">\n" <<endl;
        cout << p();
        cout << "<h2> Brookes Job Portal </h2> \n" <<endl; 
        cout << p();
        cout << "Register account" << endl;
        cout << "</div>\n" << endl;
        //add username
        cout << form().set("method", "post").set("action", "/register/confirm.cgi") << endl;
        cout << "<div style=\"width:100%; text-align: center; display: inline-block;\">" << endl;
        cout << p("Username").set("class", "lable") << endl;
        cout << input().set("type", "text").set("name", "username").set("class", "input") << endl;
        cout << "<br>" << endl;
        //add password
        cout << p("Password").set("class", "lable") << endl;
        cout << input().set("type", "password").set("name", "password").set("class", "input") << endl;
        cout << "<br>" << endl;
        // add em@il
        cout << p("Email").set("class", "lable") << endl;
        cout << input().set("type", "email").set("name", "email").set("class", "input") << endl;
        cout << "<br>" << endl;
        // add type of user
        cout << p("Type of user").set("class", "lable") << endl;
        cout << input().set("type", "text").set("name", "type").set("class", "input") << endl;
        cout << "<br>" << endl;
        //ad name of user
        cout << p("Name").set("class", "lable") << endl;
        cout << input().set("type", "text").set("name", "name").set("class", "input") << endl;
        cout << "<br>" << endl;

        // check if the form is empty
        if (!message.empty()) {
            cout << p(message).set("class", "error") << endl;
            cout << "<br>" << endl;
        }
        cout << input().set("type", "submit").set("value", "Register").set("name", "type").set("class", "button") << endl;
        cout << "<br>" << endl;
        cout << form() << endl;
        cout << body() << endl;
        cout << html();
    }
    catch (exception& e) {
        cout << "This is an error. Please try again reloading." << endl; 
    }
}

//Main()--> call firstPage()
int main()
{
    string error = "";
    form_iterator fi = cgi.getElement("error");
    if (!fi->isEmpty() && fi != (*cgi).end())
        error = **fi;
    firstPage(error);
}


