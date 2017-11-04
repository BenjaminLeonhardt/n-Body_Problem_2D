#include "stdafx.h"
#include "Vector2D.h"


using namespace std;
#pragma comment(lib, "d2d1")
#pragma comment(lib, "Dwrite.lib")
Graphics* graphics;
thread* nbodys;
bool run = true;
static mutex lockGraph;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_DESTROY) {
		lockGraph.lock();
		run = false;
		lockGraph.unlock();
		//delete nbodys;
		PostQuitMessage(0); 
		return 0;
	}

	/*if (uMsg == WM_PAINT) {
	graphics->BeginDraw();
	graphics->ClearScreen(0.0f,0.0f,0.5f);
	graphics->DrawCircle(100, 100, 50, 1.0f, 0.0, 0.0, 1.0);
	graphics->EndDraw();
	}*/

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void nproblem_thread(bool& run) {
	srand(time(NULL));
	double stauchungsVerhaeltnis = 0;

	/*********************************************************************************************************
	
	//Initialisierung der N-Bodys hier konnen nach belieben Bodys hinzugefügt werden

	Konstruktor der Bodys ist:  Vector mit X,Y koordinaten
								Vector mit geschwindikteitsangaben pro iteration
								Double für die Gravitation
	
	*********************************************************************************************************/

	vector<Body> nbodys;
	nbodys.push_back(Body(Vector2D(1050, 500), Vector2D(0, -0.2), 0.2));
	nbodys.push_back(Body(Vector2D(1000, 500), Vector2D(0, 0), 0.5));
	nbodys.push_back(Body(Vector2D(950, 500), Vector2D(0, 0.2), 0.2));

	nbodys.push_back(Body(Vector2D(1350, 900), Vector2D(0, -0.2), 0.2));
	nbodys.push_back(Body(Vector2D(1300, 900), Vector2D(0, 0), 0.5));
	nbodys.push_back(Body(Vector2D(1250, 900), Vector2D(0, 0.2), 0.2));

	nbodys.push_back(Body(Vector2D(1050, 200), Vector2D(0, -0.2), 0.2));
	nbodys.push_back(Body(Vector2D(1000, 200), Vector2D(0, 0), 0.5));
	nbodys.push_back(Body(Vector2D(950, 200), Vector2D(0, 0.2), 0.2));
	//nbodys.push_back(Body(Vector2D(650, 653), Vector2D(0, -5), 0.5));


	/*nbodys.push_back(Body(Vector2D(50, 380), Vector2D(0, 0.2),0.2));
	nbodys.push_back(Body(Vector2D(1000, 500), Vector2D(0, 0), 5));

	nbodys.push_back(Body(Vector2D(150, 250), Vector2D(0.8, 0), 0.2));
	nbodys.push_back(Body(Vector2D(850, 900), Vector2D(0, 0.7), 0.2));

	nbodys.push_back(Body(Vector2D(150, 550), Vector2D(0.2, 0.1), 0.1));
	nbodys.push_back(Body(Vector2D(250, 250), Vector2D(-0.1, 0), 0.3));
	nbodys.push_back(Body(Vector2D(850, 850), Vector2D(0, -0.2), 0.3));
	nbodys.push_back(Body(Vector2D(150, 850), Vector2D(0, 0.2), 0.3));
	nbodys.push_back(Body(Vector2D(850, 150), Vector2D(-0.3, 0.2), 0.3));
	nbodys.push_back(Body(Vector2D(150, 150), Vector2D(-0.2, 0), 0.3));/**/

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			//nbodys.push_back(Body(Vector2D((i * 50)+805, (j * 50)+270), Vector2D(0, 0), 0.5));
			nbodys.push_back(Body(Vector2D((i * 50)+ float(rand() % 2000) , (j * 50) + float(rand() % 1000)), Vector2D(0.0/*float(rand()%10)/10.f, float(rand() % 10) / 10.f)*/,0.0), 0.1));
			
		}
	}

	Vector2D vector_i_j;
	dreieck abstandZwischenBodys;
	Vector2D gravitationsrichtung;
	Vector2D Geschwindigkeitsrichtung;
	dreieck geschwindigkeit;
	Vector2D gravitationsrichtungGesamt;
	Vector2D posNeu;
	Vector2D richtung;
	double gravitationskonstante = 1.0; // Nicht korrekt laut lehrbuch!!!! richtiger wert ist: 6.67408 * pow(10,-31)

	double farbeR = 0.0;
	double farbeG = 0.0;
	double farbeB = 0.0;


	while (run) {
		lockGraph.lock();
		if (run) {
			graphics->BeginDraw();
			
			graphics->ClearScreen(0.0f, 0.0f, 0.1f);
		}
		lockGraph.unlock();

		for (int i = 0; i < nbodys.size(); i++) {
			gravitationsrichtungGesamt.x = 0;
			gravitationsrichtungGesamt.y = 0;
			for (int j = 0; j < nbodys.size(); j++) {
				if (i != j) {
					//Gravitations Vectoren berechnen und addieren
					vector_i_j = nbodys[j].position - nbodys[i].position;
					abstandZwischenBodys.ankatete = vector_i_j.x;
					abstandZwischenBodys.gegenkatete = vector_i_j.y;
					abstandZwischenBodys.hypotenuseBerechnen();
					if (abstandZwischenBodys.hypotenuse != 0) {
						//stauchungsVerhaeltnis = float(nbodys[j].anziehung) / float(abstandZwischenBodys.hypotenuse);
						stauchungsVerhaeltnis = gravitationskonstante* (float(nbodys[i].anziehung*nbodys[j].anziehung) / float(abstandZwischenBodys.hypotenuse*abstandZwischenBodys.hypotenuse));
					}
					else {
						stauchungsVerhaeltnis = 1.0;
					}					
					gravitationsrichtung.x = vector_i_j.x*stauchungsVerhaeltnis;
					gravitationsrichtung.y = vector_i_j.y*stauchungsVerhaeltnis;
					gravitationsrichtungGesamt += gravitationsrichtung;
				}
			}
			//gravitationsrichtung auf bewegungsrichtung addieren
			nbodys[i].position += gravitationsrichtungGesamt;
			nbodys[i].bewegungsRichtung += gravitationsrichtungGesamt;
			//Body i verschieben
			nbodys[i].position += nbodys[i].bewegungsRichtung;


			lockGraph.lock();
			if (run) {
				graphics->DrawCircle(nbodys[i].position.x, nbodys[i].position.y, 2.0f, 0.8f, 0.8f, 0.8f, 10.0f);
			}
			lockGraph.unlock();
		}
		lockGraph.lock();
		if (run) {
			graphics->EndDraw();
		}
		lockGraph.unlock();
	}
}



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmd, int nCmdShow) {
	WNDCLASSEX windowclass;
	ZeroMemory(&windowclass, sizeof(WNDCLASSEX));
	windowclass.cbSize = sizeof(WNDCLASSEX);
	windowclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclass.hInstance = hInstance;
	windowclass.lpfnWndProc = WindowProc;
	windowclass.lpszClassName = L"MainWindow";
	windowclass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowclass);
	RECT rect = { 0,0,1920,1000 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);
	HWND windowshandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MainWindow", L"DirectX", WS_OVERLAPPEDWINDOW, 0, 0,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, 0);

	if (!windowshandle) return -1;
	graphics = new Graphics();

	if (!graphics->Init(windowshandle)) {
		delete graphics;
		return -1;
	}

	ShowWindow(windowshandle, nCmdShow);



	MSG message;
	message.message = WM_NULL;

	nbodys = new thread(&nproblem_thread, std::ref(run));
	while (message.message != WM_QUIT) {
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			DispatchMessage(&message);
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			
		}
	}


	/*MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
	DispatchMessage(&message);
	}*/

	delete graphics;

	return 0;
}



