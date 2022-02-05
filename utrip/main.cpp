#include <iostream>
#include "Utrip.h"
#include "Handlers.h"
using namespace std;


int main(int argc, char** argv)
{
	try
	{
		const int port = 8080;
		Server server(port);
		cout << "Server connected at port: " << port << endl;
		string hotel_assets_path = string(argv[1]);
		string rating_assets_path = string(argv[2]);
		Utrip utrip;
		utrip.load_hotels(hotel_assets_path);
		utrip.load_ratings(rating_assets_path);
		MainHandler _interface(&utrip, &server);
		_interface.execute();
	}
	catch (Server::Exception e) 
	{
    	cerr << e.getMessage() << endl;
    }
	catch(const exception& e)
	{
		cerr << e.what() << '\n';
	}
	return 0;
}