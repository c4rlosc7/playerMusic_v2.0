//Carlos Andres Martinez - Cliente Broker Servidor
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
  	cout<<"Running broker with ip: " << ip <<endl;
  	context ctx;
  	socket cb(ctx, socket_type::xrep);  // socket cliente-broker  
  	cb.bind("tcp://*:5555");
  	//queue<int> queue_request;
  	int count=0;
  	message cbroker;
  	string idc;
  	int c;
  	while(true)
  	{
	  	//message cbroker;                  // recibe del cliente
		cb.receive(cbroker);
		cout << "recibe" << cbroker.parts() << "partes" << endl;
		cbroker >> idc >> c;				
		message bserver;               // envia al servidor	
		bserver << idc << c;
		if (c == 2){int index; cbroker >> index; bserver << index;}
		//queue_request.push(c);
		//cout << "length: " << queue_request.size() << endl;
		for(size_t i = 0; i < cbroker.parts(); i++) 
		{
			cout << i << cbroker.get(i) << endl;
		}	  									  						
		//bserver << text;
		cout << "envio" << bserver.parts() << "partes" << endl;
		for(size_t ii = 0; ii < bserver.parts(); ii++) {
			cout << ii << bserver.get(ii) << endl;
		}
		//Round Robin 		
		if (count == 0){
			socket sx(ctx, socket_type::xreq);  // socket (1) servidor-cliente  5559 5560 5561
			sx.connect("tcp://"+ip+":5557");	
			cout << "running server #1\n";    	
			sx.send(bserver);
			count = count + 1;
		}else if (count == 1){
			socket sx(ctx, socket_type::xreq);  // socket (2) servidor-cliente  5559 5560 5561
			sx.connect("tcp://"+ip+":5558");
			cout << "running server #2\n";    	
			sx.send(bserver);
			count = count + 1;
		}else if (count == 2){
			socket sx(ctx, socket_type::xreq);  // socket (3) servidor-cliente  5559 5560 5561
			sx.connect("tcp://"+ip+":5559");	    	
			cout << "running server #3\n";    	
			sx.send(bserver);
			count=0;
		}	 	 

  	}
	return 0;
}