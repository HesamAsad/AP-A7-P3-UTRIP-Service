#include "Handlers.h"
#include "Exceptions.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
using namespace std;

void MainHandler::execute()
{
	server->get("/", new ShowPage("static/home.html"));
	server->get("/signup", new ShowPage("static/signup.html"));
	server->get("/login", new ShowPage("static/login.html"));
	server->post("/signUp", new SignUp(utrip));
	server->post("/login", new LoginHandler(utrip));
	server->get("/homepage", new HomePage(utrip));
	server->get("/hotel", new HotelHandler(utrip));
	server->get("/logout", new LogoutHandler(utrip));
	server->get("/wallet", new ShowPage("static/wallet.html"));
	server->get("/showWallet", new WalletHandler(utrip));
	server->post("/addToWallet", new WalletAdder(utrip));
	server->post("/Filter", new StarFilterHandler(utrip));
	server->run();
}

Response* SignUp::callback(Request *req)
{
	Response* res = new Response;
	res->setHeader("Content-Type", "text/html");
	map<string, string> params;
	ostringstream body;
	params[USERNAME_ARG] = req->getBodyParam("username");
	params[PASS_ARG] = req->getBodyParam("password");
	params[EMAIL_ARG] = req->getBodyParam("email");
	try
	{
		if(req->getBodyParam("password") != req->getBodyParam("repassword"))
			throw Bad_request();
		utrip->signup_user(params);
		body << "<html>" << endl
			 << "	<head>" << endl 
			 << "		<title> Success! </title>" << endl
			 << "	<style>" << endl
			 << "		a:hover {" << endl
        	 << "		color:aqua;" << endl
        	 << "		}" << endl
			 << "	</style>" << endl
			 << "	</head>" << endl
			 << "	<body style=\"text-align:center\">" << endl
			 << "		<h3> Signup successfully completed! </h3>" << endl 
			 << "		<h3> Welcome to Utrip <span style=\"color:red\">" << req->getBodyParam("username") << "</span></h3><br/>" << endl
			 << "		<h4><a href='/homepage'/><button>HomePage</button></h4>" << endl
			 << "	</body>" << endl
			 << "</html>" << endl;
		res->setSessionId(utrip->get_session());
	}
	catch(const Bad_request& ex)
	{
		body << "<html>" << endl
			 << "	<head>" << endl 
			 << "		<title> Error! </title>" << endl
			 << "	<style>" << endl
			 << "		a:hover {" << endl
        	 << "		color:aqua;" << endl
        	 << "		}" << endl
			 << "	</style>" << endl
			 << "	</head>" << endl
			 << "	<body style=\"text-align:center\">" << endl
			 << "		<h1 style=\"color:red\"> Bad Request!</h1><br/>" << endl
			 << "		<p style=\"font:'Times New Roman'\"> <b>Note: </b> you may have entered a <b>repeated</b> username or email!<br/>or re-entered your password incorrectly!</p><br/>" << endl
			 << "		<h2><a href='/signup'/><button>Try again!</button></h2>" << endl
			 << "	</body>" << endl
			 << "</html>" << endl;
	}
	res->setBody(body.str());
	return res;
}

Response* LoginHandler::callback(Request *req)
{
	Response* res = new Response;
	res->setHeader("Content-Type", "text/html");
	map<string, string> params;
	params[USERNAME_ARG] = req->getBodyParam("username");
	params[PASS_ARG] = req->getBodyParam("password");
	ostringstream body;
	try
	{
		string sessionId = utrip->get_session();
		utrip->login_user(sessionId, params);
		body
			<< "<html>" << endl
			<< "	<head>" << endl 
			<< "		<title>Success!</title>" << endl
			<< "	</head>" << endl
			<< "	<body style=\"text-align:center\">" << endl 
			<< "		<h2 style=\"color:red\"> Login Successfully Completed!<br/>Welcome!</h2>" << endl 
			<< "		<a href=\"/homepage\"><button>Home</button></a>" << endl 
			<< "	</body>" << endl
			<< "</html>" << endl;
			res->setSessionId(sessionId);
	}
	catch(const Bad_request& ex)
	{
		body << "<html>" << endl
			 << "	<head>" << endl 
			 << "		<title> Error! </title>" << endl
			 << "	<style>" << endl
			 << "		a:hover {" << endl
        	 << "		color:aqua;" << endl
        	 << "		}" << endl
			 << "	</style>" << endl
			 << "	</head>" << endl
			 << "	<body style=\"text-align:center\">" << endl
			 << "		<h1 style=\"color:red\"> Bad Request!</h1><br/>" << endl
			 << "		<p style=\"font:'Times New Roman'\"> <b>Note: </b> Username or password is incorrect! </p><br/>" << endl
			 << "		<h2><a href='/login'/><button>Try again!</button></h2>" << endl
			 << "	</body>" << endl
			 << "</html>" << endl;
	}
	res->setBody(body.str());
	return res;
}

Response* HomePage::callback(Request* req)
{
	Response* res = new Response;
	res->setHeader("Content-Type", "text/html");
	vector<Hotel*> hotels = utrip->get_printable_hotels(req->getSessionId());
	ostringstream body;
	User* user = utrip->get_user(req->getSessionId());
	if(user ==NULL)
		body << "<html>\n\t<head>\n\t\t<title>Error!</title>\n\t</head>\n\t<body style=\"text-align:center\">\n\t\t<h1 style=\"color:red\">Bad Request!</h1>\n\t\t<a href=\"/\"><button> Home </button></a>\n\t</body>\n</html>";
	else
	{
		body
			<< "<html>" << endl 
			<< "	<head>" << endl 
			<< "		<title>Utrip Online!</title>" << endl 
			<< "	</head>" << endl 
			<< "	<body style=\"font-family:'Times New Roman'\">" << endl 
			<< "		<h1 style=\"text-align:center\">Welcome to <span style=\"color:brown\"> Utrip </span> " << user->get_username() << "! </h1>" << endl 
			<< "		<h2 style=\"text-align:right\">" << endl 
			<< "			<a href=\"/logout\"><button>Logout</button></a><a href=\"/wallet\"><button>Wallet</button></a>" << endl 
			<< "		</h2>" << endl
			<< "			<p style=\"text-align:left\"><form action='Filter' method='post'>" << endl
			<< "				Filter Hotel's stars: from <input type='number' name='from'> to <input type='number' name='to'><input type='submit' value='Filter'></form></p>" << endl
			<< "		<table border=2>" << endl
			<< "			<tr style=\"color:red; font-size:14pt\">" << endl
			<< "				<th style=\"color:black\">&nbsp;</th>" << endl
			<< "				<th>name</th>" << endl
			<< "				<th>stars</th>" << endl
			<< "				<th>city</th>" << endl
			<< "				<th>image</th>" << endl
			<< "			</tr>" << endl;
			int i = 1;
			for(Hotel* hotel : hotels)
			{
				body 
					<< "			<tr style=\"color:blue; font-size:16pt\">" << endl
					<< "				<th style=\"color:black\">" << i++ << "</th>" << endl
					<< "				<th><a href=\"/hotel?uid=" << hotel->get_uid() << "\"/>" << hotel->get_name() << "</th>" << endl
					<< "				<th width=75px style=\"color:orange\">" << hotel->get_stars() << "</th>" << endl
					<< "				<th style=\"color:green\">" << hotel->get_city() << "</th>" << endl
					<< "				<th><img height=225px width=225px src=\"" << hotel->get_image_url() << "\"></th>" << endl
					<< "			</tr>" << endl;
			}
			body
				<< "		</table>" << endl
				<< "	</body>" << endl
				<< "</html>" << endl;
	}
	res->setBody(body.str());
	return res;
}

Response* HotelHandler::callback(Request* req)
{
	Response* res = new Response;
	res->setHeader("Content-Type", "text/html");
	ostringstream body;
	Hotel* hotel = utrip->find_hotel(req->getQueryParam("uid"));
	User* user = utrip->get_user(req->getSessionId());
	if(user ==NULL)
		body << "<html>\n\t<head>\n\t\t<title>Error!</title>\n\t</head>\n\t<body style=\"text-align:center\">\n\t\t<h1 style=\"color:red\">Bad Request!</h1>\n\t\t<a href=\"/\"><button> Home </button></a>\n\t</body>\n</html>";
	else
	{
		body
			<< "<html>" << endl 
			<< "	<head>" << endl
			<< "		<title>Utrip " << hotel->get_name() << "</title>" << endl
			<< "	</head>" << endl 
			<< "	<body style=\"font-family:'Comic Sans'; text-align:center\">" << endl 
			<< "		<h2 style=\"text-align:right\"><a href=\"/logout\"><button>Logout</button></a></h2>" << endl
			<< "		<a href=\"/homepage\"><button> Back </button></a>" << endl
			<< "		<h3 style=\"color:blue; font-size:14pt; text-align:left\"> Detailed information for " << hotel->get_name() << ": </h3>" << endl 
			<< "		<img style=\"float:right\" width=40\% src=\"" << hotel->get_image_url() << "\">" << endl
			<< "		<table border=1 width=60%>" << endl 
			<< "			<tr height=50px><th>property name</th><th>" << hotel->get_name() << "</th></tr>" << endl 
			<< "			<tr height=50px><th style=\"color:blue\">hotel overview</th><th style=\"color:darkblue\">" << hotel->get_overview() << "</th></tr>" << endl 
			<< "			<tr height=50px><th style=\"color:orange\">stars</th><th style=\"color:orange\">" << hotel->get_stars() << "</th></tr>" << endl 
			<< "			<tr height=50px><th style=\"color:green\">city</th><th style=\"color:green\">" << hotel->get_city() << "</th></tr>" << endl;
		Rooms_info info = hotel->get_rooms_info();
		body
			<< "			<tr height=100px><th style=\"color:deepskyblue\">rooms</th><th style=\"color:deepskyblue\">" << endl
			<< "				<table align=center border=1 style=\"color:deepskyblue\">" << endl
			<< "					<tr height=20px><th width=30%>&nbsp</th><th width=35%>count</th><th width=35%>price</th>" << endl
			<< "					<tr height=20px><th>standard</th><th>" << info.std_count << "</th><th>" << info.std_price << "</th>" << endl
			<< "					<tr height=20px><th>deluxe</th><th>" << info.deluxe_count << "</th><th>" << info.deluxe_price << "</th>" << endl
			<< "					<tr height=20px><th>luxury</th><th>" << info.lux_count << "</th><th>" << info.lux_price << "</th>" << endl
			<< "					<tr height=20px><th>premium</th><th>" << info.prem_count << "</th><th>" << info.prem_price << "</th>" << endl
			<< "				</table>" << endl 
			<< "			</th></tr>" << endl
			<< "			<tr height=50px><th style=\"color:brown\">Amenities</th><th style=\"color:brown\">" << endl
			<< "									<ul style=\"color:brown\">" << endl;
		vector<string> amenities = Utrip::split(hotel->get_amenities(), '|');
		for(string& amenity : amenities)
			body << "								<li>" << amenity << "</li>" << endl;
		body
			<< "							</ul>" << endl
			<< "						</ul>" << endl 
			<< "		</table>" << endl
			<< "	<br/>" << endl
			<< "	</body>" << endl 
			<< "</html>" << endl;
	}
	res->setBody(body.str());
	return res;
}

Response* LogoutHandler::callback(Request* req)
{
	Response* res = new Response;
	res->setHeader("Content-Type", "text/html");
	ostringstream body;
	utrip->logout_user(req->getSessionId());
	body
		<< "<html>" << endl
		<< "	<head>" << endl 
		<< "		<title>Success!</title>" << endl
		<< "	</head>" << endl
		<< "	<body style=\"text-align:center\">" << endl 
		<< "		<h2 style=\"color:red\"> Logout Successfully Completed! </h2>" << endl 
		<< "		<a href=\"/\"><button>Home</button></a>" << endl 
		<< "	</body>" << endl
		<< "</html>" << endl;
	
	res->setBody(body.str());
	return res;
}

Response* WalletHandler::callback(Request* req)
{
	Response* res = new Response;
	res->setHeader("Content-Type", "text/html");
	string wallet = utrip->get_wallet(req->getSessionId(), req->getBodyParam("count"));
	vector<string> partial_wallet = Utrip::split(wallet, '\n');
	User* user = utrip->get_user(req->getSessionId());
	ostringstream body;
	body
		<< "<html>" << endl 
		<< "	<head>" << endl
		<< "		<title> Utrip Wallet </title>" << endl 
		<< "	</head>" << endl 
		<< "	<body style=\"font:'Comic Sans'; text-align:center\">" << endl 
		<< "		<h1>" << user->get_username() << "'s Wallet</h1><br/>" << endl
		<< "		<h2> current Amount: " << partial_wallet.front() << "$<h2><br/>" << endl
		<< "		<table border=1 style=\"margin-left:auto;margin-right:auto\">" << endl
		<< "			<tr height=30px><th width=200px>&nbsp</th><th width=200px style=\"color:blue\">amount</th></tr>" << endl;
		int i = 1;
	for(string& wallet_amount : partial_wallet)
		body << "			<tr height=30px><th>" << i++ << "</th><th style=\"color:blue\">" << wallet_amount << "$</th></tr>" << endl;
	body
		<< "		</table>" << endl
		<< "		<a href=\"/wallet\"><button> Back </button></a>" << endl
		<< "		<br/>" << endl
		<< "		<a href=\"/homepage\"><button> Home </button></a>" << endl
		<< "	</body>" << endl 
		<< "</html>" << endl;
	res->setBody(body.str());
	return res;
}

Response* WalletAdder::callback(Request* req)
{
	Response* res = new Response;
	res->setHeader("Content-Type", "text/html");
	string amount_s = req->getBodyParam("amount");
	ostringstream body;
	if(amount_s.empty())
	{
		body
			 << "<html>" << endl
			 << "	<head>" << endl 
			 << "		<title> Error! </title>" << endl
			 << "	<style>" << endl
			 << "		a:hover {" << endl
        	 << "		color:aqua;" << endl
        	 << "		}" << endl
			 << "	</style>" << endl
			 << "	</head>" << endl
			 << "	<body style=\"text-align:center\">" << endl
			 << "		<h1 style=\"color:red\"> Bad Request!</h1><br/>" << endl
			 << "		<p style=\"font:'Times New Roman'\"> <b>Note: </b>Amount cannot be Empty!</p><br/>" << endl
			 << "		<a href='/wallet'><button>Try again!</button></a>" << endl
			 << "	</body>" << endl
			 << "</html>" << endl;
	}
	else
	{
		utrip->add_to_wallet(req->getSessionId(), stod(amount_s));
		body
			 << "<html>" << endl
			 << "	<head>" << endl 
			 << "		<title> Success! </title>" << endl
			 << "	</head>" << endl
			 << "	<body style=\"text-align:center\">" << endl
			 << "		<h2 style=\"color:red\"> Amount Successfully added!</h2><br/>" << endl
			 << "		<a href='/showWallet'><button>See Your wallet!</button></a>" << endl
			 << "		<a href='/homepage'><button>Home</button></a>" << endl
			 << "	</body>" << endl
			 << "</html>" << endl;
	}
	res->setBody(body.str());
	return res;
}

Response* StarFilterHandler::callback(Request* req)
{
	Response* res = new Response;
	res->setHeader("Content-Type", "text/html");
	ostringstream body;
	if(req->getBodyParam("from").empty() || req->getBodyParam("to").empty())
	{
		body << "<html>\n\t<head>\n\t\t<title>Error!</title>\n\t</head>\n\t<body style=\"text-align:center\">\n\t\t<h1 style=\"color:red\">Bad Request!</h1>\n\t\t<a href=\"/\"><button> Home </button></a>\n\t</body>\n</html>";
	}
	else
	{
		int from = stoi(req->getBodyParam("from"));
		int to = stoi(req->getBodyParam("to"));
		utrip->stars_filter(req->getSessionId(), from, to);
		body << "<html>\n\t<head>\n\t\t<title>Success!</title>\n\t</head>\n\t<body style=\"text-align:center\">\n\t\t<h1 style=\"color:red\">Filter Successfully Applied!</h1>\n\t\t<a href=\"/homepage\"><button> Home </button></a>\n\t</body>\n</html>";
	}
	res->setBody(body.str());
	return res;
}