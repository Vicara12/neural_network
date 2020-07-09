#include <iostream>

class Padre
{
public:
   virtual void metodo () const = 0;
private:
   Padre ();
};

class Test : public Padre
{
public:
   static std::string prueba ();
   void metodo () const {}
private:
   Test ();
};

std::string Test::prueba ()
{
   return "yay";
}

int main ()
{
   std::string message = Test::prueba();
}