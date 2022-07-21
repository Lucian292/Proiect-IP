#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stack>
#include <bits/stdc++.h>
#include <graphics.h>
#include <math.h>

using namespace std;

struct Leksema //structura ce descrie orice numar sau operatie
{
	char type; // 0 pt numere, "+" pentru adunare ș.a.m.d.
	long double value; //valoare (doar pt numere), operatiile au valoare 0
};

char vch[100]="",sep[]=" ", RezultatFinal[500];
char sirprelucrat[500]="", sirnumar[100]="";
int i=0,j=0, numere[100],contor1=0,contor2=0, aparitii_zero=0;
bool VerificareNumar=false, VerificareParanteza=false, VerificareParantezaPtSemne=false, DupaVirgula=true, zero, raport=true;
string numar;
stack <Leksema> Stack_n; //stiva cu numere
stack <Leksema> Stack_o; //stiva cu operatii
Leksema item; //obiect de tip Leksema
int operand=0, precedent=0, intermediar=0, inceputa=0, contor=0, in, distanta=0;
char propozitie[500], *p, semn;

////^^^^^^^^^^^^^^^^^^^^^/////
////Variabilele utilizate/////
//////////////////////////////

double Sin(double x) { //rotunjim valoarea sinusului
	return (round(sin(x) * 10000000) / 10000000);
}

double Cos(double x) { //rotunjim valoarea cosinusului
	return (round(cos(x) * 10000000) / 10000000);
}

double ctg(double x) { //functie pt calcului cotangentei
	double a = Cos(x);
	double b = Sin(x);
	return (a / b);
}

int getRang(char Ch) { //functia returneaza prioritatea operatiei: 1 pt adunare si scadere, 2 pt inmultire si impartire, etc.
    if (Ch == 's' || Ch == 'c' || Ch == 't' || Ch == 'g')return 4;
    if (Ch == '^')return 3;
	if (Ch == '+' || Ch == '-')return 1;
	if (Ch == '*' || Ch == '/')return 2;
	else return 0;
}

bool Maths(stack <Leksema>& Stack_n, stack <Leksema>& Stack_o, Leksema& item) { //functie matematica ce efectueaza calcule
	//functia are tipul bool pt a putea intoarce "false" la aparitia vreunei erori
	double a, b, c;
	a = Stack_n.top().value; //se ia numarul din varful stivei
	Stack_n.pop(); //se sterge numarul din varful stivei
	switch (Stack_o.top().type) {  //se verifica tipul operatiei din varsul stivei cu operatii
	case '+': //daca tipul operatiei din varful stivei este adunare
		b = Stack_n.top().value;
		Stack_n.pop();
		c = a + b;
		item.type = '0';
		item.value = c;
		Stack_n.push(item); //rezultatul operatiei se insereaza inapoi in stiva
		Stack_o.pop();
		break;

	case '-':
		b = Stack_n.top().value;
		Stack_n.pop();
		c = b - a;
		item.type = '0';
		item.value = c;
		Stack_n.push(item);
		Stack_o.pop();
		break;

	case '*':
		b = Stack_n.top().value;
		Stack_n.pop();
		c = a * b;
		item.type = '0';
		item.value = c;
		Stack_n.push(item);
		Stack_o.pop();
		break;

    case '^':
		b = Stack_n.top().value;
		Stack_n.pop();
		c = pow(b, a);
		item.type = '0';
		item.value = c;
		Stack_n.push(item);
		Stack_o.pop();
		break;

	case '/':
		b = Stack_n.top().value;
		if (a == 0) {
			cerr << "\nNu putem imparti la 0!\n";
			return false;
		}
		else {
			Stack_n.pop();
			c = (b / a);
			item.type = '0';
			item.value = c;
			Stack_n.push(item);
			Stack_o.pop();
			break;
		}

	case 's':
		c = Sin(a);
		item.type = '0';
		item.value = c;
		Stack_n.push(item);
		Stack_o.pop();
		break;

	case 'c':
		c = Cos(a);
		item.type = '0';
		item.value = c;
		Stack_n.push(item);
		Stack_o.pop();
		break;

	case 't':
		if (Cos(a) == 0) {
			cerr << "\nArgument gresit pentru tangent!\n";
			return false;
		}
		else {
			c = tan(a);
			item.type = '0';
			item.value = c;
			Stack_n.push(item);
			Stack_o.pop();
			break;
		}

	case 'g':
		if (Sin(a) == 0) {
			cerr << "\nArgument gresit pentru cotangent!\n";
			return false;
		}
		else {
			c = ctg(a);
			item.type = '0';
			item.value = c;
			Stack_n.push(item);
			Stack_o.pop();
			break;
		}

    default:
        cerr << "\nEroare!\n";
        return false;
        break;
        }
    return true;
}

////^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^////
////Functiile pentru calculul prin metoda 1 ////

//////////////////////////////////////////////////////////////////
///////////Transformarea din numar in cuvinte/////////////////////
// \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ ///

bool verificareIntreg(string a, int lungime)
{
    for (int i = 0; i < lungime; ++i)
        if (a[i] == ',' || a[i] == '.')
            return false;
    return true;
}

bool verificarePerioada(string a, int lungime)
{
    for (int i = 0; i < lungime; ++i)
        if (a[i] == '(')
            return true;
    return false;
}

string formatareZerouri(string a, int lungime)
{
    string format = "";
    for (int i = 0; i < a.size(); ++i)
    {
        if (a[i] != '0') break;
        else format += a[i];
    }
    return format;
}

string formatareNumarIntreg(string a, int lungime)
{
    if (verificareIntreg(a, lungime)) return a;
    string format = "";
    for (int i = 0; i < lungime; ++i)
        if (a[i] == '.' || a[i] == ',') return format;
        else format += a[i];
}

string formatareDupaVirgula(string a, int lungime)
{
    string formatAuxiliar = formatareNumarIntreg(a, lungime);
    int numarIntregLungime = formatAuxiliar.size();
    string format = "";
    for (int i = numarIntregLungime + 1; i < lungime; ++i)
        if (a[i] == '(') return format;
        else format += a[i];
    return format;
}

string formatarePerioada(string a, int lungime)
{
    int poz = lungime;
    string format = "";
    for (int i = 0; i < lungime; ++i)
        if (a[i] == '(')
        {
            poz = i + 1;
            break;
        }
    for (int i = poz; i < lungime - 1; ++i)
        format += a[i];
    return format;
}

int numarCifre(string a, int Start, int Final)
{
    return Final - Start;
}

void afisareSpeciala(string a, int Start, int Final)
{
    switch (a[Start])
    {
    case '0':
        strcat(RezultatFinal, "");
        break;
    case '1':
        strcat(RezultatFinal, " o");
        break;
    case '2':
        strcat(RezultatFinal, " doua");
        break;
    case '6':
        strcat(RezultatFinal, " sai");
    }

}

void afisareCifra(string a, int Start, int Final)
{
    switch (a[Final])
    {
    case '0':
        strcat(RezultatFinal, " zero");
        break;
    case '1':
        strcat(RezultatFinal, " unu");
        break;
    case '2':
        strcat(RezultatFinal, " doi");
        break;
    case '3':
        strcat(RezultatFinal, " trei");
        break;
    case '4':
        strcat(RezultatFinal, " patru");
        break;
    case '5':
        strcat(RezultatFinal, " cinci");
        break;
    case '6':
        strcat(RezultatFinal, " sase");
        break;
    case '7':
        strcat(RezultatFinal, " sapte");
        break;
    case '8':
        strcat(RezultatFinal, " opt");
        break;
    case '9':
        strcat(RezultatFinal, " noua");
        break;
    }
}

void afisareSpeciala2(string a, int Start, int Final)
{
    switch (a[Start])
    {
    case '0':
        strcat(RezultatFinal, "");
        break;
    default:
        strcat(RezultatFinal, " si");
        afisareCifra(a, Start, Start);
        break;
    }
}

void afisareZecilor(string a, int Start, int Final)
{
    if (a[Start] == '1')
    {
        switch (a[Final])
        {
        case '0':
            strcat(RezultatFinal, " zece");
            break;
        case '1':
            strcat(RezultatFinal, " unsprezece");
            break;
        case '2':
            strcat(RezultatFinal, " doisprezece");
            break;
        case '3':
            strcat(RezultatFinal, " treisprezece");
            break;
        case '4':
            strcat(RezultatFinal, " paisprezece");
            break;
        case '5':
            strcat(RezultatFinal, " cincisprezece");
            break;
        case '6':
            strcat(RezultatFinal, " saisprezece");
            break;
        case '7':
            strcat(RezultatFinal, " saptesprezece");
            break;
        case '8':
            strcat(RezultatFinal, " optsprezece");
            break;
        case '9':
            strcat(RezultatFinal, " nouasprezece");
            break;
        }
    }
    else
    {
        switch (a[Start])
        {
        case '2':
            afisareSpeciala(a, Start, Start);
            strcat(RezultatFinal, "zeci");
            afisareSpeciala2(a, Final, Final);
            break;
        case '3':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal, "zeci");
            afisareSpeciala2(a, Final, Final);
            break;
        case '4':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal, "zeci");
            afisareSpeciala2(a, Final, Final);
            break;
        case '5':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal, "zeci");
            afisareSpeciala2(a, Final, Final);
            break;
        case '6':
            afisareSpeciala(a, Start, Start);
            strcat(RezultatFinal, "zeci");
            afisareSpeciala2(a, Final, Final);
            break;
        case '7':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal, "zeci");
            afisareSpeciala2(a, Final, Final);
            break;
        case '8':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal, "zeci");
            afisareSpeciala2(a, Final, Final);
            break;
        case '9':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal, "zeci");
            afisareSpeciala2(a, Final, Final);
            break;
        }
    }
}

void afisareSutelor(string a, int Start, int Final)
{
    int nonZerouriPozitie = Final + 1;
    for (int i = Start + 1; i <= Final; ++i)
        if (a[i] != '0')
        {
            nonZerouriPozitie = i;
            break;
        }
    switch (a[Start])
    {
    case '1':
        afisareSpeciala(a, Start, Start);
        strcat(RezultatFinal," suta");
        break;
    case '2':
        afisareSpeciala(a, Start, Start);
        strcat(RezultatFinal," sute");
        break;
    case '3': afisareCifra(a, Start, Start);
        strcat(RezultatFinal," sute");
        break;
    case '4':
        afisareCifra(a, Start, Start);
        strcat(RezultatFinal," sute");
        break;
    case '5':
        afisareCifra(a, Start, Start);
        strcat(RezultatFinal," sute");
        break;
    case '6':
        afisareCifra(a, Start, Start);
        strcat(RezultatFinal," sute");
        break;
    case '7':
        afisareCifra(a, Start, Start);
        strcat(RezultatFinal," sute");
        break;
    case '8':
        afisareCifra(a, Start, Start);
        strcat(RezultatFinal," sute");
        break;
    case '9':
        afisareCifra(a, Start, Start);
        strcat(RezultatFinal," sute");
        break;
    }

    string nou = "";
    Start = nonZerouriPozitie;
    for (int i = Start; i <= Final; ++i)
        nou = nou + a[i];
    int lungime_nou = nou.size();

    switch (lungime_nou)
    {
    case 1:
        afisareCifra(nou, 0, lungime_nou - 1);
        break;
    case 2:
        afisareZecilor(nou, 0, lungime_nou - 1);
        break;
    }
}

void afisareMiilor(string a, int Start, int Final)
{
    int nonZerouriPozitie = Final + 1;
    for (int i = Start + (Final - 2); i <= Final; ++i)
        if (a[i] != '0')
        {
            nonZerouriPozitie = i;
            break;
        }
    switch (Final + 1)
    {
    case 4:
        switch (a[Start])
        {
        case '1':
            afisareSpeciala(a, Start, Start);
            strcat(RezultatFinal," mie");
            break;
        case '2':
            afisareSpeciala(a, Start, Start);
            strcat(RezultatFinal," mii");
            break;
        case '3':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal," mii");
            break;
        case '4':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal," mii");
            break;
        case '5':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal," mii");
            break;
        case '6':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal," mii");
            break;
        case '7':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal," mii");
            break;
        case '8':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal," mii");
            break;
        case '9':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal," mii");
            break;
        }
        break;
    case 5:
        switch (a[Start])
        {
        case '1':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," mii");
            break;
        case '2':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de mii");
            break;
        case '3':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de mii");
            break;
        case '4':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de mii");
            break;
        case '5':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de mii");
            break;
        case '6':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de mii");
            break;
        case '7':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de mii");
            break;
        case '8':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de mii");
            break;
        case '9':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de mii");
            break;
        }
        break;
    case 6:
        switch (a[Start])
        {
        case '1':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," mii");
            else
                strcat(RezultatFinal," de mii");
            break;
        case '2':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," mii");
            else
                strcat(RezultatFinal," de mii");
            break;
        case '3':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," mii");
            else
                strcat(RezultatFinal," de mii");
            break;
        case '4':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," mii");
            else
                strcat(RezultatFinal," de mii");
            break;
        case '5':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," mii");
            else
                strcat(RezultatFinal," de mii");
            break;
        case '6':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," mii");
            else
                strcat(RezultatFinal," de mii");
            break;
        case '7':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," mii");
            else
                strcat(RezultatFinal," de mii");
            break;
        case '8':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," mii");
            else
                strcat(RezultatFinal," de mii");
            break;
        case '9':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," mii");
            else
                strcat(RezultatFinal," de mii");
            break;
        }
        break;
    }
    string nou = "";
    Start = nonZerouriPozitie;
    for (int i = Start; i <= Final; ++i)
        nou = nou + a[i];
    int lungime_nou = nou.size();

    switch (lungime_nou)
    {
    case 1:
        afisareCifra(nou, 0, lungime_nou - 1);
        break;
    case 2:
        afisareZecilor(nou, 0, lungime_nou - 1);
        break;
    case 3:
        afisareSutelor(nou, 0, lungime_nou - 1);
        break;
    }
}

void afisareMilioanelor(string a, int Start, int Final)
{
    int nonZerouriPozitie = Final + 1;
    for (int i = Start + (Final - 5); i <= Final; ++i)
        if (a[i] != '0')
        {
            nonZerouriPozitie = i;
            break;
        }
    switch (Final + 1)
    {
    case 7:
        switch (a[Start])
        {
        case '1':
            strcat(RezultatFinal," un milion");
            break;
        case '2':
            afisareSpeciala(a, Start, Start);
            strcat(RezultatFinal," milioane");
            break;
        case '3':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal, " milioane");
            break;
        case '4':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal," milioane");
            break;
        case '5':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal," milioane");
            break;
        case '6':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal," milioane");
            break;
        case '7':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal," milioane");
            break;
        case '8':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal," milioane");
            break;
        case '9':
            afisareCifra(a, Start, Start);
            strcat(RezultatFinal," milioane");
            break;
        }
        break;
    case 8:
        switch (a[Start])
        {
        case '1':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," milioane");
            break;
        case '2':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de milioane");
            break;
        case '3':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de milioane");
            break;
        case '4':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de milioane");
            break;
        case '5':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de milioane");
            break;
        case '6':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de milioane");
            break;
        case '7':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de milioane");
            break;
        case '8':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de milioane");
            break;
        case '9':
            afisareZecilor(a, Start, Start + 1);
            strcat(RezultatFinal," de milioane");
            break;
        }
        break;
    case 9:
        switch (a[Start])
        {
        case '1':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," milioane");
            else
                strcat(RezultatFinal," de milioane");
            break;
        case '2':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," milioane");
            else
                strcat(RezultatFinal," de milioane");
            break;
        case '3':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," milioane");
            else
                strcat(RezultatFinal," de milioane");
            break;
        case '4':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," milioane");
            else
                strcat(RezultatFinal," de milioane");
            break;
        case '5':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," milioane");
            else
                strcat(RezultatFinal," de milioane");
            break;
        case '6':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," milioane");
            else
                strcat(RezultatFinal," de milioane");
            break;
        case '7':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," milioane");
            else
                strcat(RezultatFinal," de milioane");
            break;
        case '8':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," milioane");
            else
                strcat(RezultatFinal," de milioane");
            break;
        case '9':
            afisareSutelor(a, Start, Start + 2);
            if (a[Start + 1] == '1' || (a[Start + 1] == '0' && a[Start + 2] != '0'))
                strcat(RezultatFinal," milioane");
            else
                strcat(RezultatFinal," de milioane");
            break;
        }
        break;
    }
    string nou = "";
    Start = nonZerouriPozitie;
    for (int i = Start; i <= Final; ++i)
        nou = nou + a[i];
    int lungime_nou = nou.size();

    switch (lungime_nou)
    {
    case 1:
        afisareCifra(nou, 0, lungime_nou - 1);
        break;
    case 2:
        afisareZecilor(nou, 0, lungime_nou - 1);
        break;
    case 3:
        afisareSutelor(nou, 0, lungime_nou - 1);
        break;
    case 4:
        afisareMiilor(nou, 0, lungime_nou - 1);
        break;
    case 5:
        afisareMiilor(nou, 0, lungime_nou - 1);
        break;
    case 6:
        afisareMiilor(nou, 0, lungime_nou - 1);
        break;
    }
}

void afisareNumar(string a, int lungime)
{
    switch (lungime)
    {
    case 1:
        afisareCifra(a, 0, lungime - 1);
        break;
    case 2:
        afisareZecilor(a, 0, lungime - 1);
        break;
    case 3:
        afisareSutelor(a, 0, lungime - 1);
        break;
    case 4:
        afisareMiilor(a, 0, lungime - 1);
        break;
    case 5:
        afisareMiilor(a, 0, lungime - 1);
        break;
    case 6:
        afisareMiilor(a, 0, lungime - 1);
        break;
    case 7:
        afisareMilioanelor(a, 0, lungime - 1);
        break;
    case 8:
        afisareMilioanelor(a, 0, lungime - 1);
        break;
    case 9:
        afisareMilioanelor(a, 0, lungime - 1);
        break;
    }

}

////^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^///
///////////Transformarea din numar in cuvinte/////////////////////
//////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
///////////Transformarea din cuvinte in numar/////////////////////
// \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ ///

void cifre(char *p, int &operand, int &precedent)
{
    char cuvant[100];
    strcpy(cuvant,"zero");
    if (strcmp(p,cuvant)==0)
        precedent=precedent+0;
    strcpy(cuvant,"unu");
    if (strcmp(p,cuvant)==0)
        precedent=precedent+1;
    strcpy(cuvant,"doi");
    if (strcmp(p,cuvant)==0)
        precedent=precedent+2;
    strcpy(cuvant,"doua");
    if (strcmp(p,cuvant)==0)
        precedent=precedent+2;
    strcpy(cuvant,"trei");
    if (strcmp(p,cuvant)==0)
        precedent=precedent+3;
    strcpy(cuvant,"patru");
    if (strcmp(p,cuvant)==0)
        precedent=precedent+4;
    strcpy(cuvant,"cinci");
    if (strcmp(p,cuvant)==0)
        precedent=precedent+5;
    strcpy(cuvant,"sase");
    if (strcmp(p,cuvant)==0)
        precedent=precedent+6;
    strcpy(cuvant,"sapte");
    if (strcmp(p,cuvant)==0)
        precedent=precedent+7;
    strcpy(cuvant,"opt");
    if (strcmp(p,cuvant)==0)
        precedent=precedent+8;
    strcpy(cuvant,"noua");
    if (strcmp(p,cuvant)==0)
        precedent=precedent+9;

}

void zecimale(char*p, int &operand, int &precedent)
{
    char cuvant[100];
    short int gasit=0;
    strcpy(cuvant, "zece");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+10;
        gasit=1;
    }
    strcpy(cuvant, "unsprezece");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+11;
        gasit=1;
    }
    strcpy(cuvant, "doisprezece");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+12;
        gasit=1;
    }
    strcpy(cuvant, "treisprezece");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+13;
        gasit=1;
    }
    strcpy(cuvant, "paisprezece");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+14;
        gasit=1;
    }
    strcpy(cuvant, "cincisprezece");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+15;
        gasit=1;
    }
    strcpy(cuvant, "saisprezece");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+16;
        gasit=1;
    }
    strcpy(cuvant, "saptesprezece");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+17;
        gasit=1;
    }
    strcpy(cuvant, "optsprezece");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+18;
        gasit=1;
    }
    strcpy(cuvant, "nouasprezece");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+19;
        gasit=1;
    }
    strcpy(cuvant, "douazeci");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+20;
        gasit=1;
    }
    strcpy(cuvant, "treizeci");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+30;
        gasit=1;
    }
    strcpy(cuvant, "patruzeci");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+40;
        gasit=1;
    }
    strcpy(cuvant, "cincizeci");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+50;
        gasit=1;
    }
    strcpy(cuvant, "saizeci");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+60;
        gasit=1;
    }
    strcpy(cuvant, "saptezeci");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+70;
        gasit=1;
    }
    strcpy(cuvant, "optzeci");
    if (strcmp(p,cuvant)==0)
    {
        precedent=precedent+80;
        gasit=1;
    }
    strcpy(cuvant, "nouazeci");
    if (strcmp(p,"nouazeci")==0)
    {
        precedent=precedent+90;
        gasit=1;
    }
    if (gasit==0)
        cifre(p,operand,precedent);
}

void sute(char *p, int &operand, int &precedent, int &intermediar)
{
    char cuvant[100];
    int gasit=0;
    strcpy(cuvant, "suta");
    if (strcmp(p,cuvant)==0)
    {
        intermediar=100;
        gasit=1;
        precedent=0;
    }
    strcpy(cuvant, "sute");
    if (strcmp(p,cuvant)==0)
    {
        intermediar=precedent*100;
        gasit=1;
        precedent=0;
    }

    if (gasit==0)
        zecimale(p,operand,precedent);
}

void mii(char *p, int &operand, int &precedent, int &intermediar)
{
    char cuvant[100];
    int gasit=0;
    strcpy(cuvant, "mie");
    if (strcmp(p,cuvant)==0)
    {
        operand=operand+1000;
        gasit=1;
    }
    strcpy(cuvant, "mii");
    if (strcmp(p,cuvant)==0 && intermediar!=0 || strcmp(p,cuvant)==0 && precedent!=0)
    {
        if (precedent!=0)
        {
            intermediar=intermediar+precedent;
            precedent=0;
        }
        operand=operand+intermediar*1000;
        intermediar=0;
        gasit=1;
    }
    if (gasit==0)
        sute(p,operand,precedent,intermediar);
}

void milioane(char *p, int &operand, int &precedent, int &intermediar)
{
    char cuvant[100];
    int gasit=0;
    strcpy(cuvant, "milion");
    if (strcmp(p,cuvant)==0)
    {
        operand=operand+1000000;
        gasit=1;
    }
    strcpy(cuvant, "milioane");
    if (strcmp(p,cuvant)==0 && intermediar!=0 || strcmp(p,cuvant)==0 && precedent!=0)
    {
        if (precedent!=0)
        {
            intermediar=intermediar+precedent;
            precedent=0;
        }
        operand=operand+intermediar*1000000;
        gasit=1;
        intermediar=0;
    }
    if (gasit==0)
        mii(p,operand,precedent,intermediar);
}

////^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^////
///////////Transformarea din cuvinte in numar/////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
/////////principalele functii pentru prelucrare si calcul/////////
//// \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ ////


void verificare(char *p) //se inlatura cuvintele inutile din propozitia introdusa
{
    if (strstr(p,"suma")||strstr(p,"adunat") || strstr(p,"adunarea") || strstr(p,"adunarii") || strstr(p,"sumei"))
        {
        strcat(sirprelucrat,"+");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"plus")||strstr(p,"adunat"))
        {
        strcat(sirprelucrat,"plus");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"scader")||strstr(p,"-") || strstr(p,"diferenta")|| strstr(p,"diferentei"))
        {
        strcat(sirprelucrat,"-");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"minus"))
        {
        strcat(sirprelucrat,"minus");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"produs") || strstr(p,"inmultir"))
        {
        strcat(sirprelucrat,"*");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"inmultit")||strstr(p,"ori"))//dupa cuvantul "inmultit" se va pune cuvantul "la"
        {
        strcat(sirprelucrat,"ori");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"putere"))
        {
        strcat(sirprelucrat,"^");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"raport") || strstr(p,"impartirea") || strstr(p,"impartirei")||strstr(p,"impartirii"))
        {
        strcat(sirprelucrat,"/");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"impartit"))
        {
        strcat(sirprelucrat,"impartit");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"sin"))
        {
        strcat(sirprelucrat,"s");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"cos"))
        {
        strcat(sirprelucrat,"c");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"tan"))
        {
        strcat(sirprelucrat,"t");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"cot"))
        {
        strcat(sirprelucrat,"g");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"("))
        {
        strcat(sirprelucrat,"(");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,")"))
        {
        strcat(sirprelucrat,")");
        strcat(sirprelucrat,sep);
        }
    if (strstr(p,"unu") || strstr(p,"doi") || strstr(p,"trei") || strstr(p,"patru") || strstr(p,"cinci")||strstr(p,"sase")||strstr(p,"sapte")||strstr(p,"opt")||strstr(p,"noua")||
        strstr(p,"zero")||strstr(p,"doua")||strstr(p,"zece")||strstr(p,"douazeci")||strstr(p,"treizeci")||strstr(p,"patruzeci")||strstr(p,"cincizeci")||strstr(p,"saizeci")||
        strstr(p,"saptezeci")||strstr(p,"optzeci")||strstr(p,"nouazeci")||strstr(p,"sut")||strstr(p,"mie")||strstr(p,"mii")||strstr(p,"milion")||strstr(p,"milioane")||
        strstr(p,"unsprezece")||strstr(p,"doisprezece")||strstr(p,"treisprezece")||strstr(p,"paisprezece")||strstr(p,"cincisprezece")||strstr(p,"saisprezece")||
        strstr(p,"saptesprezece") || strstr(p,"optsprezece") || strstr(p,"nouasprezece")||strstr(p,"cu"))
        {if (strstr(p,",")) {
            p[strlen(p)-1]=NULL;
            strcat(sirprelucrat,p);
            strcat(sirprelucrat,sep);
            strcat(sirprelucrat, ",");
            strcat(sirprelucrat,sep);
            }
            else
            {
                strcat(sirprelucrat,p);
                strcat(sirprelucrat,sep);
            }
        }
    if (strstr(p,"?"))
        {
        strcat(sirprelucrat,"?");
        strcat(sirprelucrat,sep);
        }
}

void PrelucrareaPropozitiei (int variabila)
{

    p = strtok(propozitie,sep);
    while (p)
    {
        verificare(p);
        p = strtok(NULL, sep);
    }
    //cout<<sirprelucrat<<endl;
    p = strtok(sirprelucrat,sep);
    while (p)
    {
        if (strstr(p,"unu") || strstr(p,"doi") || strstr(p,"trei") || strstr(p,"patru") || strstr(p,"cinci") || strstr(p,"sase") || strstr(p,"sapte") || strstr(p,"opt") || strstr(p,"noua") ||
        strstr(p,"zero") || strstr(p,"doua") || strstr(p,"zece") || strstr(p,"douazeci") || strstr(p,"treizeci") || strstr(p,"patruzeci") || strstr(p,"cincizeci") || strstr(p,"saizeci") ||
        strstr(p,"saptezeci")||strstr(p,"optzeci")||strstr(p,"nouazeci")||strstr(p,"sut")||strstr(p,"mie")||strstr(p,"mii")||strstr(p,"milion")||strstr(p,"milioane")||
        strstr(p,"unsprezece") || strstr(p,"doisprezece") || strstr(p,"treisprezece") || strstr(p,"paisprezece") || strstr(p,"cincisprezece") || strstr(p,"saisprezece") ||
        strstr(p,"saptesprezece") || strstr(p,"optsprezece") || strstr(p,"nouasprezece"))
        {
            if (strstr (p,"zero"))
                zero=true;
            VerificareNumar=true;
            milioane(p,operand,precedent,intermediar);
        }
        else
        {
            if (strstr(p, "+")) { vch[i]='+';i++;}
            if (strstr(p, "-")) { vch[i]='-';i++;}
            if (strstr(p, "*")) { vch[i]='*';i++;}
            if (strstr(p, "/")) { vch[i]='/';i++;}
            if (strcmp(p, "s")==0) { vch[i]='s';i++;}
            if (strcmp(p, "c")==0) { vch[i]='c';i++;}
            if (strcmp(p, "t")==0) { vch[i]='t';i++;}
            if (strcmp(p, "g")==0) { vch[i]='g';i++;}
            if (strstr(p, "(")) { vch[i]='(';i++;}
            if (strstr(p,",")) if (VerificareNumar) {vch[i]='n';i++;}
            if (strstr(p,")"))
            {
                VerificareParanteza=true;
                VerificareParantezaPtSemne=true;
                distanta=1;
                if (VerificareNumar)
                {
                    vch[i]='n';i++;
                    vch[i]=')';i++;
                }
                else {vch[i]=')'; i++;}
            }
            if (strstr(p,"?")) if (VerificareNumar) {vch[i]='n';i++;}
            if (strstr(p, "plus"))
            {
                if (VerificareNumar)
                {
                    vch[i]='n';i++; vch[i]='+';i++;
                }
                if (VerificareParantezaPtSemne && distanta==1)
                {
                    {vch[i]='+'; i++; VerificareParantezaPtSemne=false; distanta=0;}
                }
                else if (distanta>2) {distanta=0; VerificareParantezaPtSemne=false;}
            }
            if (strstr(p, "minus"))
            {
                if (VerificareNumar)
                {
                    vch[i]='n';i++; vch[i]='-';i++;
                }
                if (VerificareParantezaPtSemne && distanta==1)
                {
                    {vch[i]='-'; i++; VerificareParantezaPtSemne=false; distanta=0;}
                }
                else if (distanta>2) {distanta=0; VerificareParantezaPtSemne=false;}
            }
            if (strstr(p, "ori"))
            {
                if (VerificareNumar)
                {
                    vch[i]='n';i++; vch[i]='*';i++;
                }
                if (VerificareParantezaPtSemne && distanta==1)
                {
                    {vch[i]='*'; i++; VerificareParantezaPtSemne=false; distanta=0;}
                }
                else if (distanta>2) {distanta=0; VerificareParantezaPtSemne=false;}
            }
            if (strstr(p, "impartit"))
            {
                if (VerificareNumar)
                {
                    vch[i]='n';i++; vch[i]='/';i++;
                }
                if (VerificareParantezaPtSemne && distanta==1)
                {
                    {vch[i]='/'; i++; VerificareParantezaPtSemne=false; distanta=0;}
                }
                else if (distanta>2) {distanta=0; VerificareParantezaPtSemne=false;}
            }
            if (strstr(p, "^"))
            {
                if (VerificareNumar)
                {
                    vch[i]='n';i++;vch[i]='^';i++; goto IesireDinIf;
                }
                if (VerificareParantezaPtSemne && distanta==1)
                {
                    {vch[i]='^'; i++; VerificareParantezaPtSemne=false; distanta=0;goto IesireDinIf;}
                }
                else {vch[i]='^'; i++;}
            }
            IesireDinIf:
            if (strstr(p,"cu"))
            {
                if(VerificareParanteza) goto OmitemParanteza;
                if (VerificareNumar) {vch[i]='n';i++;}
            }
            OmitemParanteza:
            VerificareParanteza=false;
            VerificareNumar=false;
            if (precedent!=0)
                operand=operand+precedent;
            if (intermediar!=0)
                operand=operand+intermediar;
            if (operand!=0 || zero)
            {
                zero=false;
                numere[contor]=operand;
                contor++;
                operand=0;
                precedent=0;
                intermediar=0;
            }

        }
        p=strtok(NULL, sep);
    }

            if (precedent!=0)
                operand=operand+precedent;
            if (intermediar!=0)
                operand=operand+intermediar;
            if (operand!=0||zero)
            {
                zero=false;
                numere[contor]=operand;
                contor++;
                operand=0;
                precedent=0;
                intermediar=0;
            }

}

////////////////////////////////////////////////////////////
////////////Calcularea sirului cu operator in fata//////////
// \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ ///

double calcul(int rezultat)
{
        double nr=-1, rez_impartire;
        bool primu=true;
        while(contor1<i)
        {
            if (vch[contor1]=='(')
            {
                contor1++;
                calcul(rezultat);
            }
            if (vch[contor1]=='+')
            {
                contor1++;
                if (vch[contor1]=='n')
                {
                    nr=numere[contor2];
                    contor1++;
                    contor2++;
                    IesireSuma:
                    while (vch[contor1]=='n')
                    {
                        nr=nr+numere[contor2];
                        contor1++;
                        contor2++;
                    }
                   // cout<<nr<<endl;
                }
                if (vch[contor1]=='(')
                {
                    if (nr==-1 && primu)
                        {
                            nr=0;
                            primu=false;
                        }
                        contor1++;
                        nr=nr+calcul(rezultat);
                        goto IesireSuma;
                }
                if (vch[contor1]==')')
                {
                    contor1++;
                    return nr;
                }
                return nr;
            }
            if (vch[contor1]=='-')
            {
                contor1++;
                if (vch[contor1]=='n')
                {
                    nr=numere[contor2];
                    contor1++;
                    contor2++;
                    IesireDiferenta:
                    while (vch[contor1]=='n')
                    {
                        nr=nr-numere[contor2];
                        contor1++;
                        contor2++;
                    }
                  //  cout<<nr<<endl;
                }
                if (vch[contor1]=='(')
                {
                    if (nr==-1 && primu)
                        {
                            contor1++;
                            nr=calcul(rezultat);
                            primu=false;
                            goto PrimuElementDiferenta;
                        }
                    contor1++;
                    nr=nr-calcul(rezultat);
                    PrimuElementDiferenta:
                    goto IesireDiferenta;
                }
                if (vch[contor1]==')')
                {
                    contor1++;
                    return nr;
                }
                return nr;
            }
            if (vch[contor1]=='*')
            {
                contor1++;
                if (vch[contor1]=='n')
                {
                    nr=numere[contor2];
                    contor1++;
                    contor2++;
                    IesireProdus:
                    while (vch[contor1]=='n')
                    {
                        nr=nr*numere[contor2];
                        contor1++;
                        contor2++;
                    }
                //    cout<<nr<<endl;
                }
                if (vch[contor1]=='(')
                {
                    if (nr==-1 && primu)
                        {
                            nr=1;
                            primu=false;
                        }
                        contor1++;
                        nr=nr*calcul(rezultat);
                        goto IesireProdus;
                }
                if (vch[contor1]==')')
                {
                    contor1++;
                    return nr;
                }
                return nr;
            }
            if (vch[contor1]=='/')
            {
                contor1++;
                if (vch[contor1]=='n')
                {
                    nr=numere[contor2];
                    contor1++;
                    contor2++;
                    IesireRaport:
                    while (vch[contor1]=='n')
                    {
                        if (numere[contor2]==0 && aparitii_zero==0)
                        {
                            cout<<"Nu este posibila impartirea la 0";
                            raport=false;
                            aparitii_zero++;
                        }
                        nr=nr/numere[contor2];
                        contor1++;
                        contor2++;
                    }
                  //  cout<<nr<<endl;
                }
                if (vch[contor1]=='(')
                {
                        if (nr==-1 && primu)
                        {
                            contor1++;
                            nr=calcul(rezultat);
                            primu=false;
                            goto PrimuElementRaport;
                        }
                        contor1++;
                        rez_impartire=calcul(rezultat);
                        if (rez_impartire==0 && primu==false && aparitii_zero==0)
                        {
                            cout<<"Nu este posibila impartirea la 0";
                            raport=false;
                            aparitii_zero++;
                        }
                        nr=nr/rez_impartire;
                        rez_impartire=-1;
                        PrimuElementRaport:
                        goto IesireRaport;
                }
                if (vch[contor1]==')')
                {
                    contor1++;
                    return nr;
                }
                return nr;
            }
            if (vch[contor1]=='^')
            {
                contor1++;
                if (vch[contor1]=='n')
                {
                    nr=numere[contor2];
                    contor1++;
                    contor2++;
                    IesirePutere:
                    while (vch[contor1]=='n')
                    {
                        nr=pow(nr,numere[contor2]);
                        contor1++;
                        contor2++;
                    }
                  //  cout<<nr<<endl;
                }
                if (vch[contor1]=='(')
                {
                    if (nr==-1 && primu)
                    {
                        contor1++;
                        nr=calcul(rezultat);
                        primu=false;
                        goto PrimuElementPutere;
                    }
                    contor1++;
                    nr=pow(nr,calcul(rezultat));
                    PrimuElementPutere:
                    goto IesirePutere;
                }
                if (vch[contor1]==')')
                {
                    contor1++;
                    return nr;
                }
                return nr;
            }
        }
}

///////////////////////////////////////////////////////////
/////////Calcularea sirului introdus matematic/////////////
// \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ //

double CalculareaRezultatului (double variabila)
{
        if (vch[0]=='+'||vch[0]=='-'||vch[0]=='*'||vch[0]=='/'||vch[0]=='^')
    {
        numar = to_string(calcul(1));
        //cout<<numar<<endl;

    }
    else{
    i=0;j=0;

    char Ch; //variabila in care se inscrie caracterul curent prelucrat
	double value;

	while (1) { //Rezolvarea exercitiului prin metoda 1
		Ch = vch[i]; i++; //verificam primul simbol
		if (Ch == '\n')break; //daca ajungem la sfarsitul randului iesim din while
		if (Ch == ' ') { //ignorarea spatiilor
			//cin.ignore();
			continue;
		}

			if (Ch == 's') { //daca am citit functia sin
				item.type = 's';
				item.value = 0;
				Stack_o.push(item); //operatia se insereaza in stiva cu operatii
				continue;
			}
			if (Ch == 'c') { //daca am citit functia cos
				item.type = 'c';
				item.value = 0;
				Stack_o.push(item); //operatia se insereaza in stiva cu operatii
				continue;
			}
			if (Ch == 't') { //daca am citit functia tangenta
				item.type = 't';
				item.value = 0;
				Stack_o.push(item); //operatia se insereaza in stiva cu operatii
				continue;
			}
			if (Ch == 'g') { //daca am citit functia cotangenta
				item.type = 'g';
				item.value = 0;
				Stack_o.push(item); //operatia se insereaza in stiva cu operatii
				continue;
			}
		if (Ch == 'n') { //daca am citit un numar
			value=numere[j]; j++;
			item.type = '0';
			item.value = value;
			Stack_n.push(item); //numarul se insereaza in stiva cu numere
			continue;
		}
		if (Ch == '+' || Ch == '-' || Ch == '*' || Ch == '/' || Ch == '^') { //daca am citit o operatie
			if (Stack_o.size() == 0) { //daca stiva cu operatii este vida
				item.type = Ch;
				item.value = 0;
				Stack_o.push(item); //operatia se insereaza in stiva cu operatii
				//cin.ignore();
				continue;
			}
			if (Stack_o.size() != 0 && getRang(Ch) > getRang(Stack_o.top().type)) { //daca stiva nu este vida, insa prioritatea operatiei curente este mai mare decat cea din varful stivei
				item.type = Ch;
				item.value = 0;
				Stack_o.push(item); //operatia se insereaza in stiva cu operatii
				//cin.ignore();
				continue;
			}
			if (Stack_o.size() != 0 && getRang(Ch) <= getRang(Stack_o.top().type)) {//daca stiva nu este vida, insa prioritatea operatiei curente e mai mica sau egala cu cea din varful stivei
				if (Maths(Stack_n, Stack_o, item) == false) { //daca funtia returneaza 'false' incetam lucrul
					system("pause");
					return 0;
				}
				item.type = Ch;
				item.value = 0;
				Stack_o.push(item); //operatia se insereaza in stiva cu operatii
				//cin.ignore();
				continue;
			}
		}
		if (Ch == '(') { //daca am citit paranteza deschisa
			item.type = Ch;
			item.value = 0;
			Stack_o.push(item); //operatia se insereaza in stiva cu operatii
			//cin.ignore();
			continue;
		}
		if (Ch == ')') { //daca am citit paranteza inchisa
			while (Stack_o.top().type != '(') {
				if (Maths(Stack_n, Stack_o, item) == false) { //daca functia returneaza 'false' incetam calculul
					system("pause");
					return 0;
				}
				else continue; //daca totul e bine
			}
			Stack_o.pop();
            //cin.ignore();
			continue;
		}
		else { //daca am citit un caracter straniu
			//cout << "\nAti introdus expresia gresit\n";
			//system("pause");
			break;//return 0;
		}
    }
    while (Stack_o.size() != 0) { //apelam functia matematica pana cand in stiva cu operatii nu raman 0 elemente
    if (Maths(Stack_n, Stack_o, item) == false) { //daca functia returneaza 'false' incetam calculul
        system("pause");
        return 0;
    }
    else continue; //daca totul e bine
	}
	//cout << "Raspunsul: " << Stack_n.top().value << endl; //afisam rezultatul

    numar = to_string(Stack_n.top().value);
    //cout<<numar<<endl;
    }
}

void TransformareNumarInCuvinte (int variabila)
{
    //string numar;//=to_string(Stack_n.top().value);
    if (numar[0] == '-')
    {
        strcat(RezultatFinal, " minus");
        numar.erase(0, 1);
    }

    int lungime = numar.size();
    //cout<<lungime<<endl;
    lungime=lungime-1;
        while (numar[lungime]=='0' || numar[lungime]=='.' || numar[lungime]==',') //se inlatura zerourile in plus de dupa virgula
        {
            //cout<<"Valoarea actuala: "<<numar[lungime]<<endl;
            if (numar[lungime]=='.' || numar[lungime]==',')
            {
                    numar.erase(lungime,1);
                    goto Virgula;
            }
            numar.erase(lungime,1);
            //cout<<numar<<endl;
            lungime--;
        }
    Virgula:
    //cout<<"Numar final: "<<numar<<endl;
    //cout<<lungime<<endl;
    lungime=lungime+1;
    //cout<<"Lungime finala: "<<lungime<<endl;

    string numarIntreg = formatareNumarIntreg(numar, lungime);
    string numarDupaVirgula = formatareDupaVirgula(numar, lungime);
    string perioada = formatarePerioada(numar, lungime);
    afisareNumar(numarIntreg, numarIntreg.size());
    if (!verificareIntreg(numar, lungime))
    {
        strcat(RezultatFinal, " virgula");
        int lungimeNumarDupaVirgula = numarDupaVirgula.size();
        while (lungimeNumarDupaVirgula)
        {
            string zerouri = formatareZerouri(numarDupaVirgula, numarDupaVirgula.size());
            int lungimeZerouri = zerouri.size();
            while (lungimeZerouri--)
                strcat(RezultatFinal, " zero");
            numarDupaVirgula.erase(0, zerouri.size());
            lungimeNumarDupaVirgula = numarDupaVirgula.size();
            int lungimeNou = min(lungimeNumarDupaVirgula, 9);
            string nou = "";
            for (int i = 0; i < lungimeNou; ++i)
                nou += numarDupaVirgula[i];
            afisareNumar(nou, nou.size());
            numarDupaVirgula.erase(0, nou.size());
            lungimeNumarDupaVirgula = numarDupaVirgula.size();
        }

        if (verificarePerioada(numar, lungime))
        {
            strcat(RezultatFinal, " si");
            int lungimeNumarPerioada = perioada.size();
            while (lungimeNumarPerioada)
            {
                string zerouri = formatareZerouri(perioada, perioada.size());
                int lungimeZerouri = zerouri.size();
                while (lungimeZerouri--)
                    strcat(RezultatFinal, " zero");
                perioada.erase(0, zerouri.size());
                lungimeNumarPerioada = perioada.size();
                int lungimeNou = min(lungimeNumarPerioada, 9);
                string nou = "";
                for (int i = 0; i < lungimeNou; ++i)
                    nou += perioada[i];
                afisareNumar(nou, nou.size());
                perioada.erase(0, nou.size());
                lungimeNumarPerioada = perioada.size();
            }
            strcat(RezultatFinal, " in perioada");
        }
    }
}

////^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^////
/////////principalele functii pentru prelucrare si calcul/////////
//////////////////////////////////////////////////////////////////

void graphwindowINFO(int window)
{
    int windowI,x,y;
    bool click=false;
    windowI=initwindow(800,600,"CalcRO INFO");
    readimagefile("Informatie.jpg",0,0,800,600);

    do{
        if(ismouseclick(WM_LBUTTONDOWN)){
        clearmouseclick(WM_LBUTTONDOWN);
        x=mousex(); y=mousey();
        if(x>626 && x<785 && y>462 && y<512) {closegraph(windowI); click=true;}
        }
    }while (!click);
    setcurrentwindow(window);
}

int main()
{
    int window, x,y;
    bool click;
    window=initwindow(800,600,"CalcRO");

    readimagefile("Meniu.jpg",0,0,800,600);

    do{

        if(ismouseclick(WM_LBUTTONDOWN)){
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex(); y=mousey();
            if(x>65 && x<270 && y>64 && y<130) {closegraph(window); click=true; }
            else if(x>64 && x<467 && y>177 && y<250) graphwindowINFO(window);
                else if(x>66 && x<211 && y>295 && y<352){
                    closegraph(window);
                    goto closeprogram;
                    }
        }
    } while(!click);

    cout<<"Introduceti propozitia: "<<endl;

    cin.getline(propozitie,500);

    for(i=0;i<strlen(propozitie);i++)
        propozitie[i]=tolower(propozitie[i]);
    i=0;
    PrelucrareaPropozitiei (1);

    CalculareaRezultatului (1);

    TransformareNumarInCuvinte (1);

    if (raport)
        cout<<"Raspunsul:"<<RezultatFinal<<endl;

    closeprogram:
    return 0;
}
