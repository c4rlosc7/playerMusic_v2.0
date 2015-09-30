#include <iostream>
#include <string>
#include <cassert>
#include <zmqpp/zmqpp.hpp>
using namespace std;
using namespace zmqpp;
/*
class server{
	private:
		string name;
		string artist;
		string filename;
	public:	
		song(){}  
		song(const string &name1, const string &artist1, const string &filename1){			
			name = name1;			
			artist = artist1;
			filename = filename1;
		}
		const string& getname() const { 
			return name; 
		}		
};
*/
int main(int argc, char **argv)
{
  string ip; 		
  ip = argv[1];
  cout << "Servidor\n";
  context ctx;
  socket bs(ctx, socket_type::xrep);  // socket broker-server  
  bs.bind("tcp://*:5557");    
  socket sc(ctx, socket_type::xreq);  // socket servidor-cliente
  sc.connect("tcp://"+ip+":5559");
  while(true)  	
  	{  	  		  	
	    message bserver;                // recibe del broker
	    bs.receive(bserver);
	    cout << "recibe" << bserver.parts() << "partes" << endl;
	    string idc, idb, text;
	    bserver >> idc >> idb >> text;
		for(size_t i = 0; i < bserver.parts(); i++) {
			cout << i << bserver.get(i) << endl;
		}		    	
	    message sclient;                    // envia al cliente
	    sclient << idc << idb << text;	    
	    cout << "envio" << sclient.parts() << "partes" << endl;
		for(size_t ii = 0; ii < sclient.parts(); ii++) {
			cout << ii << sclient.get(ii) << endl;
		}	    
	    sc.send(sclient);	    
  	}
	return 0;
}