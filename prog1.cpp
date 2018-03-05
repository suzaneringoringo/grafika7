#include <cstdlib>
#include <iostream>

int main() {
   int input;

   std::cout << "1.Font Bitmap - Dot" << "\n";
   std::cout << "2.Plane & Missiles - Line" << "\n";
   std::cout << "3.Font Vector - Polygon & Fill" << "\n";
   std::cout << "4.Plane - Scaling and Rotation" << "\n";
   std::cout << "5.Plane - Viewport" << "\n";
   std::cout << "6.Map - Viewport & Circle" << "\n\n";
   
   std::cout << "Input number? " << "\n";
   std::cin >> input;
   if(input == 1){	
   	std::system("./1_font");
   }else if(input == 2){
   	std::system("./2_plane");
   }else if(input == 3){
   	std::system("./3_font");
   }else if(input == 4){
   	std::system("./4_scaling_rotation");
   }else if(input == 5){
   	std::system("./5_viewport");
   }else if(input == 6){
   	std::system("./6_circle");
   }else{
   	std::system("./prog2");
   }
   return 0;
}