#include <iostream>
#include <string>
#include <cassert>
#include <queue> 
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

int main(int argc, char **argv)
{
  string ip; 		
  ip = argv[1];
  cout<<"Broker\n";
  context ctx;
  socket cb(ctx, socket_type::xrep);  // socket cliente-broker  
  cb.bind("tcp://*:5555");
  socket bs(ctx, socket_type::xreq);  // socket broker-server  
  bs.connect("tcp://"+ip+":5557");
  //cb.bind("tcp://*:" + port);
  //poller p;
  //p.add(bs, poller::poll_in);
  queue<string> queue_request;
  while(true)
  {
	message cbroker;                  // recibe del cliente
	cb.receive(cbroker);
	cout << "recibe" << cbroker.parts() << "partes" << endl;
	string idc, text;
	cbroker >> idc >> text;
	
	queue_request.push(text);
	cout << "length: " << queue_request.size() << endl;

	for(size_t i = 0; i < cbroker.parts(); i++) {
		cout << i << cbroker.get(i) << endl;
	}	

	message bserver;               // envia al servidor
	bserver << idc;
	bserver << text;
	cout << "envio" << bserver.parts() << "partes" << endl;
	for(size_t ii = 0; ii < bserver.parts(); ii++) {
		cout << ii << bserver.get(ii) << endl;
	}
	bs.send(bserver);
	//string msg_server;
	//cout << "ingrese msg servidor: ";
	//cin >> msg_server;
	//bserver << id;
	//bserver << msg_server;		
/*
	  if(p.poll())
	  {
	  	while(p.has_input(bs))     
	  	{			
	  	}
	  }  	
*/	  
  }
/*   
  while(true)
  	{ 

	    message cbroker;                  // recibe del cliente
	    cb.receive(cbroker);
	    string id;
	    cbroker >> id;
	    string text;
	    cbroker >> text;
	    cout << text << endl;
	  
	    message bservidor;               // envia al servidor
	    string msg_servidor;
	    cout << "ingrese msg servidor: ";
	    cin >> msg_servidor;
	    //bservidor << id;
	    bservidor << msg_servidor;
	    bs.send(bservidor);
	}
*/
	return 0;
}