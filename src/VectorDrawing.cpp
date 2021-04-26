#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "./Format/CBitmap.h"
#include "./Shapes/CDrawing.h"


void Draw(CDrawing *Drawing){
  cout << "(II) P_Bitmap exection start (" << __DATE__ << " - " << __TIME__ << ")" << endl;
  cout << "(II) CBitmap object creation" << endl;
  CBitmap *image = new CBitmap();

  string filename2 = Drawing->_filename.substr(0, Drawing->_filename.find(".")) + ".bmp";
  remove(filename2.c_str());
  cout << "(II) CImage pointer extraction" << endl;
  //if(Drawing->_img != NULL){
  delete Drawing->_img;
  //}
  Drawing->CreateImage(Drawing->_maxX, Drawing->_maxY, Drawing->_r_backgnd, Drawing->_g_backgnd, Drawing->_b_backgnd);
  Drawing->DrawImage();

  image->setImage( Drawing->_img );
  cout << "(II) CBitmap image saving" << endl;
  image->SaveBMP(filename2);

  delete image;
}

void Info(CDrawing *Drawing){
    cout << endl;
    cout << "##----" << " PROPERTIES OF : "<< Drawing->_filename.substr(0, Drawing->_filename.find(".")) << " ----## " << endl;

    cout << "SIZE : "  << Drawing->_maxX  << " x " << Drawing->_maxY << " pixels" << endl;
    cout << "PLANS : " << Drawing->_maxZ  << endl;
    cout << "SCALE : " << Drawing->_scale << endl;
    cout << Drawing->_size << " shapes" << endl;
    cout << "------------------------------" << endl;;
}

string newShapeCommand(){
  string type;
  string red;
  string green;
  string blue;
  string opacity;
  string x;
  string y;
  string z;

  cout << ">>> New Shape creation :" << endl;
  cout << ">>> Choose type : [POINT, LINE, RECTANGLE(_F), DISK(_F)]" << endl;
  cin  >> type;

  cout << ">>> Choose color :" << endl;
  cout << " > RED = " ;
  cin  >> red;
  cout << " > GREEN = " ;
  cin  >> green;
  cout << " > BLUE = " ;
  cin  >> blue;
  cout << " > OPACITY = ";
  cin  >> opacity;
  cout << endl;

  cout << ">>> Choose location :" << endl;
  cout << " > X = ";
  cin  >> x;
  cout << " > Y = ";
  cin  >> y;
  cout << " > Z = ";
  cin  >> z;
  cout << endl;

  string command;
  if(type == "POINT"){
    command = type+" : "+x+", "+y+", "+z+", "+red+", "+green+", "+blue+", "+opacity+";";
  }
  else if(type == "RECTANGLE" || type == "RECTANGLE_F"){
    string width, height;
    cout << ">>> Choose parameters : " << endl;
    cout << " > WIDTH : ";
    cin  >> width;
    cout << " > HEIGHT : ";
    cin  >> height;
    cout << endl;
    command = type+" : "+x+", "+y+", "+width+", "+height+", "+z+", "+red+", "+green+", "+blue+", "+opacity+";";
  }
  else if(type == "DISK" || type == "DISK_F"){
    string radius;
    cout << ">>> Choose parameters : " << endl;
    cout << " > RADIUS : ";
    cin  >> radius;
    cout << endl;
    command = type+" : "+x+", "+y+", "+radius+", "+z+", "+red+", "+green+", "+blue+", "+opacity+";";
  }
  else if(type == "LINE"){
    string x2, y2;
    cout << ">>> Choose parameters : " << endl;
    cout << " > X2 : ";
    cin  >> x2;
    cout << " > Y2 : ";
    cin  >> y2;
    cout << endl;
    command = type+" : "+x+", "+y+", "+x2+", "+y2+", "+z+", "+red+", "+green+", "+blue+", "+opacity+";";
  }
  return(command);
}

int ShapesMenu(CDrawing *Drawing){
  string option;
  while(1){
    cout << "\033[41m";
    cout << " \033[42m\033[30mV";
    cout << "\033[43mD";
    cout << "\033[44m";
    cout << " \033[0m>>> ";
    cout << Drawing->_filename.substr(0, Drawing->_filename.find("."));
    cout << " >> SHAPES >>" ;//| ADD | REMOVE | LIST | BACK" << endl;
    cout << " | \033[47m\033[30m ADD \033[0m";
    cout << " | \033[47m\033[30m REMOVE \033[0m";
    cout << " | \033[47m\033[30m LIST \033[0m";
    cout << " | \033[43m\033[30m BACK \033[0m" << endl;

    cin >> option;
    if (option == "ADD"){
      string command = newShapeCommand();
      Drawing->addShape(command);
    }
    else if (option == "REMOVE"){
      int index;
      Drawing->showShapes();
      cout << ">>> Shape to remove (Index) : ";
      cin  >> index;
      Drawing->removeShape(index);
      cout << ">>> Shape n°" << index << " removed from drawing" << endl;
    }
    else if (option == "LIST"){
      Drawing->showShapes();
    }
    else if(option == "BACK"){
      return 0;
    }
  }
}

int ParameterStatus(string option, string filename){
  int status = 0;
  ifstream infile;
  infile.open(filename);
  ofstream outfile;
  outfile.open(filename+".temp", fstream::trunc);
  string STRING;
  while(not(infile.eof())){
    getline(infile, STRING);
    outfile << STRING << endl;
    if (STRING.substr(0, STRING.find(":")-1) == "//"+option){
      status = 0;
    }else if(STRING.substr(0, STRING.find(":")-1) == option){
      status = 1;
    }
  }
  infile.close();
  outfile.close();
  return status;
}

int ParameterMenu(string option, string filename){
  int status;
  while(1){
    status = ParameterStatus(option, filename);
    string option2;
    cout << "\033[41m";
    cout << " \033[42m\033[30mV";
    cout << "\033[43mD";
    cout << "\033[44m";
    cout << " \033[0m>>> ";
    cout << filename.substr(0, filename.find("."));
    cout << " >> EDIT >> " << option << " >> ";

    if(status == 1){
      cout << " | \033[42m\033[30m ON \033[0m";
    }else if(status == 0){
      cout << " | \033[41m\033[30m OFF \033[0m";
    }
    cout << " | \033[47m\033[30m SET \033[0m";
    cout << " | \033[43m\033[30m BACK \033[0m" << endl;

    cin >> option2;
    if (option2 == "ON" || option2 == "OFF"){
      cout << "Changing parameter status" << endl;
      ifstream infile;
      ofstream outfile;
      infile.open(filename+".temp");
      outfile.open(filename);
      if (infile.is_open() == false)
        exit(EXIT_FAILURE);
      if (outfile.is_open() == false)
        exit(EXIT_FAILURE);

      string STRING;
      while (not(infile.eof())){
        getline(infile, STRING);
        if ((option2 == "ON")&&(STRING.substr(0,STRING.find(":")-1) == "//"+option)){
          outfile << STRING.substr(2, STRING.size()-2) << endl;
          status = 1;
          cout << "Set " << option << " ON" << endl;
        }else if((option2 == "OFF")&&(STRING.substr(0,STRING.find(":")-1) == option)){
          outfile << "//"<< STRING << endl;
          status = 0;
          cout << "Set " << option << " OFF" << endl;
        }else{
          outfile << STRING << endl;
        }
      }
      infile.close();
      outfile.close();
    }else if( option2 == "SET"){
      return 0;
    }else if( option2 == "BACK"){
      return 1;
    }
  }
 return 0;
}

string ParameterCommand(string parameter){
  string command;
  string param;
  if( parameter == "SIZE"){
    command = "SIZE : ";
    cout << " > WIDTH : ";
    cin >> param;
    command = command+param+", ";
    cout << " > HEIGHT : ";
    cin>> param;
    command = command+param+";";
  }else if(parameter == "BACKGROUND"){
    command = "BACKGROUND : ";
    cout << " > RED : ";
    cin >> param;
    command = command+param+", ";
    cout << " > GREEN : ";
    cin >> param;
    command = command+param+", ";
    cout << " > BLUE : ";
    cin >> param;
    command = command+param+";";
  }else if(parameter == "SCALE"){
    command = "SCALE : ";
    cout << " > SCALE : ";
    cin >> param;
    command = command+param+";";
  }
  cout << command << endl;
  return command;
}


int EditMenu(CDrawing *Drawing){
  string option;
  while(1){
    cout << "\033[41m";
    cout << " \033[42m\033[30mV";
    cout << "\033[43mD";
    cout << "\033[44m";
    cout << " \033[0m>>> ";
    cout << Drawing->_filename.substr(0, Drawing->_filename.find("."));
    cout << " >> EDIT >>" ;
    cout << " | \033[47m\033[30m SIZE \033[0m";
    cout << " | \033[47m\033[30m BACKGROUND \033[0m";
    cout << " | \033[47m\033[30m SCALE \033[0m";
    cout << " | \033[43m\033[30m BACK \033[0m" << endl;

    cin >> option;
    if( option == "SIZE"){
      //int out;
      //out = ParameterMenu(option, Drawing->_filename);
      if (ParameterMenu(option, Drawing->_filename) == 0){
        string command = ParameterCommand(option);
        Drawing->SetSize(command);
      }else{}
    }
    else if( option == "BACKGROUND"){
      //int out;
      //out = ;
      if (ParameterMenu(option, Drawing->_filename) == 0){
        string command = ParameterCommand(option);
        Drawing->SetBackgnd(command);
      }else{}
    }
    else if( option == "SCALE"){
      int out;
      out = ParameterMenu(option, Drawing->_filename);
      if (out = 0){
        string command = ParameterCommand(option);
        Drawing->SetBackgnd(command);
      }else{}
    }
    else if( option == "BACK"){
      return 0;
    }
  }
}

int Menu(CDrawing *Drawing){
  string option;
  while(1){
    cout << "\033[41m";
    cout << " \033[42m\033[30mV";
    cout << "\033[43mD";
    cout << "\033[44m";
    cout << " \033[0m>>> ";
    cout << Drawing->_filename.substr(0, Drawing->_filename.find("."));
    cout << " >>"; //"| DRAW | EDIT | SHAPES | INFO | BACK" << endl;
    cout << " | \033[102m\033[30m DRAW \033[0m";
    cout << " | \033[47m\033[30m EDIT \033[0m";
    cout << " | \033[47m\033[30m SHAPES \033[0m";
    cout << " | \033[47m\033[30m INF0 \033[0m";
    cout << " | \033[43m\033[30m BACK \033[0m" << endl;
    cin >> option;
    if( option == "DRAW"){
      Draw(Drawing);
    }
    else if(option == "EDIT"){
      EditMenu(Drawing);
    }
    else if(option == "SHAPES"){
      ShapesMenu(Drawing);
    }
    else if(option == "INFO"){
      Info(Drawing);
    }
    else if(option == "BACK"){
      return 0;
    }
  }
}

int StartMenu(){
  while(1){
    string option;
    cout << "\033[41m";
    cout << " \033[42m\033[30mV";
    cout << "\033[43mD";
    cout << "\033[44m";
    cout << " \033[0m>>> ";
    cout << " | \033[47m\033[30m NEW \033[0m";
    cout << " | \033[47m\033[30m OPEN \033[0m";
    cout << " | \033[41m\033[30m QUIT \033[0m" << endl;
    cin  >> option;

    if (option == "NEW"){
      CDrawing *Drawing = new CDrawing();
      cout << "Filename : ";
      string name;
      cin >> name;
      Drawing->_filename = name+".vec";
      bool create = Drawing->CreateFile(Drawing->_filename);
      if (create == true){
        cout << ">>> " << Drawing->_filename << " created" << endl;
        Menu(Drawing);
        delete Drawing;
      }
    }
    else if (option == "OPEN"){
      CDrawing *Drawing = new CDrawing();
      cout << "Filename : ";
      string name;
      cin >> name;
      Drawing->_filename = name+".vec";
      bool load = Drawing->LoadDrawing(Drawing->_filename);
      if(load == true){
        cout << ">>> " << Drawing->_filename << " loaded" << endl;
        Menu(Drawing);
        delete Drawing;
      }
    }
    else if (option == "QUIT"){
      cout << ">>> closing application..." << endl;
      return 0;
    }
  }
}

int main(int argc, char * argv[]) {
    cout << "#------------------------------#"<<endl;
    cout << "| \033[41m";
    cout << " \033[42m";
    cout << " \033[43m";
    cout << " \033[44m";
    cout << " \033[0m";
    cout <<       "   VECTOR DRAWING  ";
    cout << " \033[44m";
    cout << " \033[43m";
    cout << " \033[42m";
    cout << " \033[41m";
    cout << " \033[0m |" << endl;
    cout << "#------------------------------#"<<endl;

    StartMenu();
    return 1;
}
