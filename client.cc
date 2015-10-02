//Carlos Andres Martinez - Cliente Broker Servidor
#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include <zmqpp/zmqpp.hpp>
#include <SFML/Audio.hpp>
using namespace std;
using namespace zmqpp;
////////////////////////////play to music 
void playermusica(string string_in, string usr){
	string songPlay = usr + "-" + "song.ogg";
	fstream ob(songPlay, ios::out);
	ob << string_in;	
	sf::Music music;
	if (!music.openFromFile(songPlay))
	    cout<<"error";//return -1; // error
	music.play();
    int i;
    cin >> i;	
    ob.close();		
}

int main(int argc, char **argv)
{	
  string ip; 		// 10.253.96.236 U
  ip = argv[1];
  cout<<"running client with ip: " << ip << endl;
  context ctx;
  socket cb(ctx, socket_type::xreq); // socket cliente-broker 5555
  cb.connect("tcp://"+ip+":5555");
  socket sc(ctx, socket_type::xrep); // socket servidor-cliente 5559
  sc.bind("tcp://*:6666");

  string username;
  cout << "Ingrese el nombre de usuario: ";
  cin >> username;

  int c;
  string keyword, keysong;
  message cbroker;

  while(true)
  	{
  		cout << "-----------------CLIENT-MUSIC-PLAYER-----------------"	<< endl;
		cout << "1.Buscar  2.Reproducir : ";
		cin >> c;

		switch(c)
		{
			case 1: 			  	
  				cout << "-----------------Buscar-----------------" << endl;  			
  				cout << "palabra a buscar: ";
  				cin >> keyword;  

  				cbroker << c;
  				cbroker << keyword;
  				cb.send(cbroker);	                  
				break;            // id, 1, keyword

			case 2:                           
				cout << "-----------------Reproducir-----------------" << endl;					
				cout << "Nombre cancion: "; 
				cin >> keysong;	
				cbroker << c;
				cbroker << keysong;
				cb.send(cbroker);	      // id, 2, keysong
				break;	
			default: cout << "Ingrese opcion correcta";
    			break;															
		}	
/*
	    message cbroker;                               // mensaje al broker
	    string msg_broker;
	    cout << "ingrese a compartir: ";
	    cin >> msg_broker;
	    cbroker << msg_broker;
	    cout << "envio" << cbroker.parts() << "partes" << endl;
	    cb.send(cbroker);
*/
	    message sclient;                               // recibe del servidor
	    sc.receive(sclient);
	    int partsmessage;
	    partsmessage = sclient.parts();
	    cout << partsmessage << endl;
	    
	    if (partsmessage == 5)  // mayor que 5 seria reproducir 
	    {
	    	string ids, idb, idc, nameSong, byteSong;           // sacamos ID client, broker, server
	    	sclient >> ids >> idb >> idc >> nameSong >> byteSong;	    	
	    	playermusica(byteSong, username);
	    } else if(partsmessage == 4){  // 5 lista
	    	for(size_t i = 0; i < sclient.parts(); i++) {
				cout << sclient.get(i) << endl;
			}
	    }
/*
	    cout << "recibe" << sclient.parts() << "partes" << endl;
		for(size_t ii = 0; ii < sclient.parts(); ii++) {
			cout << ii << sclient.get(ii) << endl;
		}	    
*/		

  	}  	
	return 0;
}