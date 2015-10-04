//Carlos Andres Martinez - Cliente Servidor - Buscar - Reproducir 
#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include <zmqpp/zmqpp.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace zmqpp;

////////////////////////////play to music 
void playermusica(string string_in, string usr, string cancion){
	string songPlay = usr + "-" + "song.ogg";
	fstream ob(songPlay, ios::out);
	ob << string_in;	
	sf::Music music;
	if (!music.openFromFile(songPlay))
	    cout<<"error";//return -1; // error
	music.play();
  cout << "play:" << cancion <<endl;
    int i;
    cin >> i;	
    ob.close();		
}

int main(int argc, char **argv){	
  string ip; 		// 10.253.96.236 U, 192.168.1.12 CASA 
  ip = argv[1];
  cout<<"running client with ip: " << ip << endl;
  context ctx;
  socket cb(ctx, socket_type::xreq); // socket cliente-broker 5555
  cb.connect("tcp://"+ip+":5555");
  socket sc(ctx, socket_type::xrep); // socket servidor-cliente 5559
  sc.bind("tcp://*:6666");

  string username;
  cout << "Nombre de usuario: ";
  cin >> username;
  int c;
  string keyword;
  string keysong;
  
  message m;                 // mensaje m
while(true)
{
	cout << "-----------------CLIENT-MUSIC-PLAYER-----------------"	<< endl;
	cout << "1.Buscar  2.Reproducir : ";
	cin >> c;
	switch(c)
	{
		case 1: 			  	
  			cout << "Esperando respuesta...\n\n";
  			m << c;
  			cb.send(m);	                  
			break;            // id, 1
		case 2:                           
			m << c;			
			int index;
			cout << "Indice: ";				
			cin >> index;
			m << index;
			cout << "Esperando respuesta...\n\n";			
			cb.send(m);	      // id, 2
			break;	
		default: cout << "Ingrese opcion correcta";
    		break;															
	}		
  message bserver;
  sc.receive(bserver);
  string idb, ids;
  bserver >> idb >> ids;
  //cout << "canciones encontradas: " << bserver.parts()-3 <<endl;
  //for(size_t ii = 0; ii < bserver.parts(); ii++) {
  //  cout << "name song["<< ii-3 << "]: "<< bserver.get(ii) << endl;
  //}
  int tarea;
  bserver >> tarea;    
  if(tarea == 1){
  	cout << "Canciones encontradas: " << bserver.parts()-3<< endl;
	for(size_t i = 3; i < bserver.parts(); i++) {
		cout << "name song[" << i-3 <<"]: " << bserver.get(i) << endl;
	}
  }else if(tarea == 2){
  	cout << "partes recibidas: " << bserver.parts() <<endl;
  	string stringsong, nombre_cancion;
  	bserver >> nombre_cancion >> stringsong;
  	playermusica(stringsong, username, nombre_cancion);
  }
}
  return 0;
}