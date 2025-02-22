#include "ecere.h"

eC_Class * class_Array_ColorAlpha;
eC_Class * class_Array_double;
eC_Class * class_Foo;
eC_Class * class_Foo_double;
eC_Class * class_Foo_String;
typedef eC_Array T(Array, ColorAlpha);
typedef eC_Array T(Array, double);
typedef eC_Map T(Map, String, ColorAlpha);
typedef eC_Instance eC_Foo;
typedef uint64_t TP(Foo, A);

struct class_members_Foo { int foo; };
eC_bool Foo_constructor(eC_Foo this)
{
   struct class_members_Foo * self = IPTR(this, Foo);
   self->foo = 5;
   return true;
}
void Foo_destructor(eC_Foo this) { }

#define FOO_TP_A_INDEX 0

void Foo_add(eC_Foo this, TP(Foo, A) value)
{
   eC_ClassTemplateArgument * tArgs = this->_class->templateArgs;
   int tCount = this->_class->templateParams.count;
   int baseParam = this->_class->numParams - tCount;
   eC_ClassTemplateArgument * tArg = tArgs ? &tArgs[baseParam + FOO_TP_A_INDEX] : null;
   eC_Class * c = null;
   void * p;
   eC_DataValue dv;
   if(tArg)
   {
      if(!tArg->dataTypeClass)
         tArg->dataTypeClass = eC_findClass(this->_class->module, tArg->dataTypeString);
      c = tArg->dataTypeClass;
   }
   if(!c) c = class_int;
   switch(c->type)
   {
      case ClassType_noHeadClass: case ClassType_normalClass:
      case ClassType_structClass: case ClassType_unionClass:
         p = pTA(void, value);
         break;
      case ClassType_bitClass: case ClassType_unitClass:
      case ClassType_enumClass: case ClassType_systemClass:
      {
         eC_Class * dc = c;
         if(c->type != ClassType_systemClass)
         {
            // NOTE: Storing Class in dataType here because we don't have a 'dataTypeClass', since Type is only used by eC compiler
            if(!c->dataType)
               c->dataType = (eC_Type *) eC_findClass(c->module, c->dataTypeString);
            if(c->dataType)
               dc = (eC_Class *)c->dataType;
         }
              if(dc == class_double)      dv.d =       dTA(value), p = &dv.d;
         else if(dc == class_float)       dv.f =       fTA(value), p = &dv.f;
         else if(dc == class_char)        dv.c =       cTA(value), p = &dv.c;
         else if(dc == class_byte)        dv.uc =      bTA(value), p = &dv.uc;
         else if(dc == class_short)       dv.s =       sTA(value), p = &dv.s;
         else if(dc == class_uint16)      dv.us =     usTA(value), p = &dv.us;
         else if(dc == class_int)         dv.i =       iTA(value), p = &dv.i;
         else if(dc == class_uint)        dv.ui =     uiTA(value), p = &dv.ui;
         else /* if(c == class_int64) */  dv.ui64 = ui64TA(value), p = &dv.ui64;
         break;
      }
   }
   eC_printLn(c, p, null);
}

int main(int argc, char *argv[])
{
   eC_Application module = eC_init(null, true, false, argc, argv);
   //C(Window) win = newi(Window);
   double d;
   eC_Foo foo;

   ecere_init(module);

   T(Array, ColorAlpha) a;
   T(Array, double) ad;
   //T(Map, String, ColorAlpha) m;

   class_Array_ColorAlpha = eC_findClass(module, "Array<ColorAlpha>");
   a = newi(Array, ColorAlpha);
   Container_add(a, ColorAlpha_from_Color(DefinedColor_red));
   Container_add(a, ColorAlpha_from_Color(DefinedColor_blue));
   //Container_add(a, COLORALPHA(255, red));
   eC_printLn(a->_class, a, null);

   class_Array_double = eC_findClass(module, "Array<double>");
   ad = newi(Array, double);
   Container_add(ad, TAd(3.14159));
   eC_printLn(ad->_class, ad, null);

   d = ((double *)IPTR(ad, Array)->array)[0];
   eC_printLn(class_double, &d, null);

   // class_Foo = registerClass(module, Foo, Instance);
   class_Foo = registerClass(module, Foo, Map<String, int>); // why is this not in double quote?
   Class_addTemplateParameter(class_Foo, "A", TemplateParameterType_type, null, null);
   Class_doneAddingTemplateParameters(class_Foo);
   addMethod(class_Foo, "add", Foo_add);

   class_Foo_double = eC_findClass(module, "Foo<A = double>");
   foo = newi(Foo, double);
   Foo_add(foo, TAd(3.14159));

   class_Foo_String = eC_findClass(module, "Foo<String>");
   foo = newi(Foo, String);
   Foo_add(foo, TAp("Hello, Templates in C!"));
   Foo_add(foo, TAp("Hello Again!"));

   eC_printLn(foo->_class, foo, null); // todo, need a OnGetString for this to work?
   return 0;
}
