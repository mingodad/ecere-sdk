#define MODULE_NAME  "HelloForm"

#include "ecere.hpp"

class HelloForm : public Window
{
public:
   Button button;

   REGISTER() { REG_Window(HelloForm); }
   CONSTRUCT(HelloForm, Window)
   {
      caption = $("Sample App using Ecere Toolkit/C++ Bindings");
      borderStyle = BorderStyle::sizable;
      clientSize = { 640, 480 };
      hasClose = true;
      hasMaximize = true;
      hasMinimize = true;
      background = SystemColor::formColor;
      font = { "Arial", 30 };

      button.parent = this;
      button.position = { 200, 200 };
      button.caption = $("Yay!!");
      button.notifyClicked = [](Window & owner, Button & btn, int x, int y, Modifiers mods) -> bool
      {
         HelloForm & self = (HelloForm &)owner;
         MessageBox msgBox;
         msgBox.caption = self.button.caption;
         msgBox.contents = $("C++ Bindings!");
         msgBox.modal();
         return true;
      };
   }

   void onRedraw(Surface & surface)
   {
      surface.writeText/*f*/(100, 100, $("Class Method!!"), 14);
   }

   /*
   REGISTER()
   {
      Window::class_registration(_class);
      register_onRedraw(_class, [](Window & w, Surface & surface) { ... });
   }
   */
};

class MyApplication : public GuiApplication
{
public:
   APP_CONSTRUCT(MyApplication, GuiApplication) { }

   void main()
   {
      HelloForm hello;

      GuiApplication::main();
   }
};

MyApplication app;

REGISTER_CLASS_DEF(MyApplication, GuiApplication, app);

REGISTER_CLASS_DEF(HelloForm, Window, app);

MAIN_DEFINITION;
