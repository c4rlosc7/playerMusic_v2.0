//Carlos Andres Martinez - Cliente Servidor
#include <iostream>  // std::cout
#include <fstream>  //std::ifstream
#include <string>
#include <list>
#include <cassert>
#include <map>
#include <zmqpp/zmqpp.hpp>
using namespace std;
using namespace zmqpp;
/////////////////CLASS-SONG-OBJECT-// name, artist, filename, Metodo string Getname
class song{

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
		const string& getartistic() const { 
			return artist; 
		}
		const string& getfilename() const { 
			return filename; 
		}				
};
/////////////////CLASS-PLAYLIST-OBJECT-// lista de canciones name, Metodo add,size
class playlist{
	private:
		//list <song> songs;  // lista de objetos de tipo song 
		map<int,song> songs;
		string name;		
	public:		
		playlist(){}
		playlist(const string &n){ 
			name = n;
		}		

		void add(song s, int indx){ //Add element at the end
		songs[indx] = s;
		}		

		int size(){ // Returns the number of elements on list
		return songs.size();
		}

		const string& getplaylist() const { 
			return name; 
		}

		song songtofind(int x){
			song songtofind1;
			map<int, song>::iterator it=songs.find(x); // buscar
			songtofind1 = (*it).second;
			return songtofind1;					
		}		

		//map<int, song> listsongs(){ }
};
////////////////////////////transfer to file 
vector<char> ReadAllBytes(const string &filename)
{
    ifstream ifs(filename, ios::binary|ios::ate); // read filename in binary and stay end file
    ifstream::pos_type pos = ifs.tellg(); // position
    vector<char>  result(pos); // create vector with size pos

    ifs.seekg(0, ios::beg); // stay begin file 
    ifs.read(&result[0], pos); //how much to read 
    return result;
}
void filetomessage(const string &filename, message &r){
	vector <char> bytes = ReadAllBytes(filename);
	r.add_raw(bytes.data(), bytes.size());
}

int main(int argc, char **argv)
{
  string ip, port; 		
  ip = argv[1];
  port = argv[2];
  cout << "Running Servidor with ip: " << ip <<", port: " << port << endl;

  song a("Milagro", "A.N.I.M.A.L", "/home/carlos/Escritorio/musica/0_Milagro.ogg"); 
  song b("Familia Es la Oportunidad", "A.N.I.M.A.L", "/home/carlos/Escritorio/musica/1_Familia_Es_la_Oportunidad_.ogg");
  song c("Los Que Marcan el Camino", "A.N.I.M.A.L", "/home/carlos/Escritorio/musica/2_Los_Que_Marcan_el_Camino.ogg");
  song d("Poder Latino", "A.N.I.M.A.L", "/home/carlos/Escritorio/musica/3_Poder_Latino.ogg");
  song e("Loco Pro", "A.N.I.M.A.L", "/home/carlos/Escritorio/musica/4_Loco_Pro.ogg");
  song f("Dejar de Ser", "A.N.I.M.A.L", "/home/carlos/Escritorio/musica/5_Dejar_de_Ser.ogg");
  playlist rock("rock"); // playlist rock 
  rock.add(a,0);   //adicionar obj tipo song a la lista rock
  rock.add(b,1);
  rock.add(c,2);
  rock.add(d,3);
  rock.add(e,4);
  rock.add(f,5);
// Nombre del map, numero de elementos del map y los elementos del map
  song nombre;  
  int indice=0;
  int longitud=rock.size();

  context ctx;
  socket sx(ctx, socket_type::xrep);         // socket broker-server  
  sx.bind("tcp://*:"+port);    
  socket sc(ctx, socket_type::xreq);         // socket servidor-cliente  5559 5560 5561
  sc.connect("tcp://"+ip+":6666");

	while(true)
	{ 
		message r;
		sx.receive(r);
		for(size_t i = 0; i < r.parts(); i++) {
			cout << r.get(i) << endl;
		}
		string idc, idb;        // id 
		r >> idc >> idb;
		int operador;     // 1 buscar, 2 reproducir   
		r >> operador;
		message scm;
		if(operador == 1){             // 1 message [id,"lista",name-song]
			cout <<"play list name: " << rock.getplaylist() << " #element: " << rock.size() << endl;			
			scm << idc;
			scm << operador;
			while(indice < longitud){
  				nombre = rock.songtofind(indice);
  				cout << "name song[" << indice <<"]: " << nombre.getname() << endl;
  				scm << nombre.getname();
  				indice++;
  			}
		  	cout << "Partes a enviar: " << scm.parts() << endl;
		  	indice=0;
		  	sc.send(scm);
		}else if(operador == 2){       // 2 message [id,"reproducir",namesong,string bytes]			
			int index;
			r >> index;
			nombre = rock.songtofind(index); // objeto encontrado 
			string nombre_cancion;
			nombre_cancion = nombre.getname(); // atributo nombre del objeto song
			cout << "name song: " << nombre_cancion << endl;
			scm << idc;
			scm << operador;

			string pathfile;
			pathfile = nombre.getfilename();
			scm << nombre_cancion;
		  	cout << "ruta: " << pathfile << endl;  
		  	filetomessage(pathfile,scm);
		  	cout << "partes envia: " << scm.parts() << endl;
			sc.send(scm);		  	
		}					
  	}		
	return 0;
}
