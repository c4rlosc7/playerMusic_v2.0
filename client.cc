#include <iostream>
#include <string>
#include <cassert>
#include <zmqpp/zmqpp.hpp>
using namespace std;
using namespace zmqpp;
int main(int argc, char **argv)
{	
  string ip; 		// 10.253.96.236 U
  ip = argv[1];
  cout<<"Client\n";
  context ctx;
  socket cb(ctx, socket_type::xreq); // socket cliente-broker 5555
  cb.connect("tcp://"+ip+":5555");
  socket sc(ctx, socket_type::xrep); // socket servidor-cliente 5559
  sc.bind("tcp://*:5559");
  while(true)
  	{
	    message cbroker;                               // mensaje al broker
	    string msg_broker;
	    cout << "ingrese a compartir: ";
	    cin >> msg_broker;
	    cbroker << msg_broker;
	    cout << "envio" << cbroker.parts() << "partes" << endl;
	    cb.send(cbroker);

	    message sclient;                               // recibe del servidor
	    sc.receive(sclient);
	    cout << "recibe" << sclient.parts() << "partes" << endl;
		for(size_t ii = 0; ii < sclient.parts(); ii++) {
			cout << ii << sclient.get(ii) << endl;
		}	    
	    string id;
	    sclient >> id;
	    string text1;
	    sclient >> text1;
	    string text2;
	    sclient >> text2;	    
	    //cout << text1 << text2 << endl;  		
  	}  	
	return 0;
}