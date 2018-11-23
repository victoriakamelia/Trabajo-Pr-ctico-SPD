#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SELECT 4
#define NONE 5
#define INGRESO_INICIAL 0
#define INGRESO_PROGRESO 1
#define INGRESO_FINALIZADO 2
#define INCREMENTAR 1
#define DECREMENTAR 0
#define D1 2
#define D2 4
#define D3 5
#define D4 6
#define P1 0
#define P2 1
#define P3 3
#include<LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int datos[4] = {-1, -1, -1, -1};
int datoSalida[7] = {0,0,0,0,0,0,0};
int ledPin[7] = {13,12,11,3,A3,A4,A5};

int readButton(int analogPin);
void dibujarDatos();
void realizarAccion(int boton);
void cambiarDato(int posicion, int operacion);
void moverPosicionDato(int direccion);
bool validarSalida();
void calcularHamming();
void encenderLeds();

int estadoIngreso;
int botonAnterior;
int botonActual;
int datoSeleccionado;

void setup()
{
  for(int i=0; i<7; i++)
  {
    pinMode(ledPin[i], OUTPUT);
  }
  datoSeleccionado = 0;
  botonAnterior = NONE;
  estadoIngreso = INGRESO_INICIAL;
  lcd.begin(16, 2);
}

void loop()
{
  dibujarDatos();
  botonActual = readButton(A0);
  if(botonAnterior != botonActual)
  {
    botonAnterior = botonActual;
    switch(estadoIngreso)
    {
      case(INGRESO_INICIAL):
        if(botonActual == SELECT)
          estadoIngreso = INGRESO_PROGRESO;
        break;
      case(INGRESO_PROGRESO):
        realizarAccion(botonActual);
        break;
      case(INGRESO_FINALIZADO):
        break;
    }
  }
}

void dibujarDatos()
{
  lcd.setCursor(0,0);
  for(int i=0; i<4; i++)
  {
    if(datos[i]<0)
      lcd.print('*');
    else
      lcd.print(datos[i]);
  }
  lcd.setCursor(0,1);
  lcd.print("    ");
  if(estadoIngreso == INGRESO_PROGRESO)
  {
    lcd.setCursor(datoSeleccionado,1);
    lcd.print('^');
  }
}

void realizarAccion(int boton)
{
  switch(boton)
  {
    case(RIGHT):
      moverPosicionDato(RIGHT);
      break;
    case(UP):
      cambiarDato(datoSeleccionado, INCREMENTAR);
      break;
    case(DOWN):
      cambiarDato(datoSeleccionado, DECREMENTAR);
      break;
    case(LEFT):
      moverPosicionDato(LEFT);
      break;
    case(SELECT):
    if(validarSalida())
    {
      calcularHamming();
      encenderLeds();
      estadoIngreso = INGRESO_FINALIZADO;
    }
      break;
  }
}

void moverPosicionDato(int direccion)
{
  if(direccion == RIGHT)
    datoSeleccionado++;
  else
    datoSeleccionado--;
  if(datoSeleccionado>3)
    datoSeleccionado = 0;
  else if(datoSeleccionado<0)
    datoSeleccionado = 3;
}

void cambiarDato(int posicion, int operacion)
{
  if(operacion == INCREMENTAR)
    datos[posicion]++;
  else
    datos[posicion]--;
  if(datos[posicion]>1)
    datos[posicion] = -1;
  else if(datos[posicion]<-1)
    datos[posicion] = 1;
}

bool validarSalida()
{
  for(int i=0; i<4; i++)
  {
    if(datos[i]<0)
      return false;
  }
  return true;
}

void calcularHamming()
{
  datoSalida[D1]=datos[0];
  datoSalida[D2]=datos[1];
  datoSalida[D3]=datos[2];
  datoSalida[D4]=datos[3];
  
  if((datoSalida[D1] && !datoSalida[D2] && !datoSalida[D4])
    || (!datoSalida[D1] && !datoSalida[D2] && datoSalida[D4])
    || (datoSalida[D1] && datoSalida[D2] && datoSalida[D4])
    || (!datoSalida[D1]&& datoSalida[D2] && !datoSalida[D4] ))
    {
       datoSalida[P1]=1;
    }
  if((datoSalida[D1] && !datoSalida[D3] && !datoSalida[D4])
    || (!datoSalida[D1] && !datoSalida[D3] && datoSalida[D4])
    || (datoSalida[D1] && datoSalida[D3] && datoSalida[D4])
    || (!datoSalida[D1]&& datoSalida[D3] && !datoSalida[D4] ))
    {
       datoSalida[P2]=1;
    }
  if((datoSalida[D2] && !datoSalida[D3] && !datoSalida[D4])
    || (!datoSalida[D2] && !datoSalida[D3] && datoSalida[D4])
    || (datoSalida[D2] && datoSalida[D3] && datoSalida[D4])
    || (!datoSalida[D2]&& datoSalida[D3] && !datoSalida[D4] ))
    {
       datoSalida[P3]=1;
    }
}

void encenderLeds()
{
  for(int i=0; i<7; i++)
  {
    digitalWrite(ledPin[i], datoSalida[i]);
  }
}
int readButton(int analogPin)
{
  int val = analogRead(analogPin);
    if(val>900) return NONE;
    if(val<50) return RIGHT;
    if(val<250) return UP;
    if(val<450) return DOWN;
    if(val<650) return LEFT;
    if(val<850) return SELECT;
}

