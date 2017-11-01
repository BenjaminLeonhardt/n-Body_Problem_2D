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
	
	*********************************************************************************************************/

	vector<Body> nbodys;
	nbodys.push_back(Body(Vector2D(50, 620), Vector2D(0, -2), 0.2));
	nbodys.push_back(Body(Vector2D(45, 615), Vector2D(0, 2.0), 0.2));
	nbodys.push_back(Body(Vector2D(845, 263), Vector2D(1.5, 2.0), 0.2));
	nbodys.push_back(Body(Vector2D(50, 380), Vector2D(0, 2),0.2));
	nbodys.push_back(Body(Vector2D(500, 500), Vector2D(0, 2), 0.2));

	nbodys.push_back(Body(Vector2D(150, 250), Vector2D(1.8, 0), 0.2));
	nbodys.push_back(Body(Vector2D(850, 900), Vector2D(0, 2.7), 0.2));

	/*nbodys.push_back(Body(Vector2D(150, 550), Vector2D(1.5, 0.5), 0.1));
	nbodys.push_back(Body(Vector2D(250, 250), Vector2D(-0.5, 0), 0.03));
	nbodys.push_back(Body(Vector2D(850, 850), Vector2D(0, -1.2), 0.03));
	nbodys.push_back(Body(Vector2D(150, 850), Vector2D(0, 0.2), 0.03));
	nbodys.push_back(Body(Vector2D(850, 150), Vector2D(-0.3, 0.2), 0.03));
	nbodys.push_back(Body(Vector2D(150, 150), Vector2D(-0.2, 0), 0.03));*/

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			//nbodys.push_back(Body(Vector2D(i * 100, j * 100), Vector2D(0, 0), 0.01));
			nbodys.push_back(Body(Vector2D(i * 100, j * 100), Vector2D(float(rand()%10)/10.f, float(rand() % 10) / 10.f), 0.01));
			
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
						stauchungsVerhaeltnis = float(nbodys[j].anziehung) / float(abstandZwischenBodys.hypotenuse);
					}
					else {
						stauchungsVerhaeltnis = 1.0;
					}					
					gravitationsrichtung.x = vector_i_j.x*stauchungsVerhaeltnis;
					gravitationsrichtung.y = vector_i_j.y*stauchungsVerhaeltnis;
					gravitationsrichtungGesamt += gravitationsrichtung;
				}
			}
			//Body i verschieben
			nbodys[i].position += gravitationsrichtungGesamt;

			//Richtung für eigene Geschwindigkeit berechnen
			//Neuen Vector zwischen beiden Bodys ermitteln
			vector_i_j = gravitationsrichtungGesamt;
			Vector2D bewegungsRichtungAlt = nbodys[i].position - nbodys[i].positionAlt;
			//um 90 Grad drehen 
			//Die viele Abfragen sind für die umrundungen einmal mit und einmal gegen den Uhrzeigersinn
			if (vector_i_j.x >= 0 && vector_i_j.y >= 0) {
				if (nbodys[i].bewegungsRichtung.x >= 0 && nbodys[i].bewegungsRichtung.y >= 0) {
					Geschwindigkeitsrichtung.x = vector_i_j.y;
					Geschwindigkeitsrichtung.y = -vector_i_j.x;
				}
				else if (nbodys[i].bewegungsRichtung.x < 0 && nbodys[i].bewegungsRichtung.y >= 0) {
					Geschwindigkeitsrichtung.x = -vector_i_j.y;
					Geschwindigkeitsrichtung.y = vector_i_j.x;
				}
				else if (nbodys[i].bewegungsRichtung.x >= 0 && nbodys[i].bewegungsRichtung.y < 0) {
					Geschwindigkeitsrichtung.x = vector_i_j.y;
					Geschwindigkeitsrichtung.y = -vector_i_j.x;
				}
				else if (nbodys[i].bewegungsRichtung.x < 0 && nbodys[i].bewegungsRichtung.y < 0) {
					double bewX = nbodys[i].bewegungsRichtung.x;
					double bewY = nbodys[i].bewegungsRichtung.y;
				
					if (nbodys[i].bewegungsRichtung.x < 0 && nbodys[i].bewegungsRichtung.y < 0) {
						if (bewX < bewY) {
							Geschwindigkeitsrichtung.x = -vector_i_j.y;
							Geschwindigkeitsrichtung.y = vector_i_j.x;
						}
						else {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
					}else if (nbodys[i].bewegungsRichtung.x > 0) {
						bewX = bewX*(-1);
						if (bewX < bewY) {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
						else {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
					}
					else if (nbodys[i].bewegungsRichtung.y > 0) {
						bewY = bewX*(-1);
						if (bewX < bewY) {
							Geschwindigkeitsrichtung.x = -vector_i_j.y;
							Geschwindigkeitsrichtung.y = vector_i_j.x;
						}
						else {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
					}									
				}
			}
			else if (vector_i_j.x < 0 && vector_i_j.y >= 0) {
				if (nbodys[i].bewegungsRichtung.x >= 0 && nbodys[i].bewegungsRichtung.y >= 0) {
					Geschwindigkeitsrichtung.x = vector_i_j.y;
					Geschwindigkeitsrichtung.y = -vector_i_j.x;
				}
				else if (nbodys[i].bewegungsRichtung.x < 0 && nbodys[i].bewegungsRichtung.y >= 0) {
					Geschwindigkeitsrichtung.x = vector_i_j.y;
					Geschwindigkeitsrichtung.y = -vector_i_j.x;
				}
				else if (nbodys[i].bewegungsRichtung.x >= 0 && nbodys[i].bewegungsRichtung.y < 0) {
					double bewX = nbodys[i].bewegungsRichtung.x;
					double bewY = nbodys[i].bewegungsRichtung.y;
					if (nbodys[i].bewegungsRichtung.x < 0) {
						bewX = bewX*(-1);
						if (bewX < bewY) {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
						else {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
					}
					else if (nbodys[i].bewegungsRichtung.y < 0) {
						bewY = bewY*(-1);
						if (bewX < bewY) {
							Geschwindigkeitsrichtung.x = -vector_i_j.y;
							Geschwindigkeitsrichtung.y = vector_i_j.x;
						}
						else {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
					}
					else if (nbodys[i].bewegungsRichtung.x > 0 && nbodys[i].bewegungsRichtung.y > 0) {
						if (bewX < bewY) {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
						else {
							Geschwindigkeitsrichtung.x = -vector_i_j.y;
							Geschwindigkeitsrichtung.y = vector_i_j.x;
						}
					}
				}
				else if (nbodys[i].bewegungsRichtung.x < 0 && nbodys[i].bewegungsRichtung.y < 0) {
					Geschwindigkeitsrichtung.x = -vector_i_j.y;
					Geschwindigkeitsrichtung.y = vector_i_j.x;
				}
			}
			else if (vector_i_j.x >= 0 && vector_i_j.y < 0) {
				if (nbodys[i].bewegungsRichtung.x >= 0 && nbodys[i].bewegungsRichtung.y >= 0) {
					Geschwindigkeitsrichtung.x = -vector_i_j.y;
					Geschwindigkeitsrichtung.y = vector_i_j.x;
				}
				else if (nbodys[i].bewegungsRichtung.x < 0 && nbodys[i].bewegungsRichtung.y >= 0) {
					double bewX = nbodys[i].bewegungsRichtung.x;
					double bewY = nbodys[i].bewegungsRichtung.y;

					if (nbodys[i].bewegungsRichtung.x < 0 && nbodys[i].bewegungsRichtung.y < 0) {
						if (bewX < bewY) {
							Geschwindigkeitsrichtung.x = -vector_i_j.y;
							Geschwindigkeitsrichtung.y = vector_i_j.x;
						}
						else {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
					}
					else if (nbodys[i].bewegungsRichtung.x > 0) {
						bewY = bewY*(-1);
						if (bewX < bewY) {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
						else {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
					}
					else if (nbodys[i].bewegungsRichtung.y > 0) {
						bewX = bewX*(-1);
						if (bewX < bewY) {
							Geschwindigkeitsrichtung.x = -vector_i_j.y;
							Geschwindigkeitsrichtung.y = vector_i_j.x;
						}
						else {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
					}
				}
				else if (nbodys[i].bewegungsRichtung.x >= 0 && nbodys[i].bewegungsRichtung.y < 0) {
					//11
					Geschwindigkeitsrichtung.x = -vector_i_j.y;
					Geschwindigkeitsrichtung.y = vector_i_j.x;
				}
				else if (nbodys[i].bewegungsRichtung.x < 0 && nbodys[i].bewegungsRichtung.y < 0) {
					//12
					Geschwindigkeitsrichtung.x = vector_i_j.y;
					Geschwindigkeitsrichtung.y = -vector_i_j.x;
				}
			}
			else if (vector_i_j.x < 0 && vector_i_j.y < 0) {
				if (nbodys[i].bewegungsRichtung.x >= 0 && nbodys[i].bewegungsRichtung.y >= 0) {
					//13
					double bewX = nbodys[i].bewegungsRichtung.x;
					double bewY = nbodys[i].bewegungsRichtung.y;
					
					if (nbodys[i].bewegungsRichtung.x < 0) {
						bewX = bewX*(-1);
						if (bewX < bewY) {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
						else {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
					}
					else if (nbodys[i].bewegungsRichtung.y<0) {
						bewY = bewY*(-1);
						if (bewX < bewY) {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
						else {
							Geschwindigkeitsrichtung.x = -vector_i_j.y;
							Geschwindigkeitsrichtung.y = vector_i_j.x;
						}
					}
					else if (nbodys[i].bewegungsRichtung.x > 0 && nbodys[i].bewegungsRichtung.y > 0) {
						if (bewX < bewY) {
							Geschwindigkeitsrichtung.x = vector_i_j.y;
							Geschwindigkeitsrichtung.y = -vector_i_j.x;
						}
						else {
							Geschwindigkeitsrichtung.x = -vector_i_j.y;
							Geschwindigkeitsrichtung.y = vector_i_j.x;
						}
					}					
				}
				else if (nbodys[i].bewegungsRichtung.x < 0 && nbodys[i].bewegungsRichtung.y >= 0) {
					//14
					Geschwindigkeitsrichtung.x = vector_i_j.y;
					Geschwindigkeitsrichtung.y = -vector_i_j.x;
				}
				else if (nbodys[i].bewegungsRichtung.x >= 0 && nbodys[i].bewegungsRichtung.y < 0) {
					//15
					Geschwindigkeitsrichtung.x = -vector_i_j.y;
					Geschwindigkeitsrichtung.y = vector_i_j.x;
				}
				else if (nbodys[i].bewegungsRichtung.x < 0 && nbodys[i].bewegungsRichtung.y < 0) {
					//16
					Geschwindigkeitsrichtung.x = -vector_i_j.y;
					Geschwindigkeitsrichtung.y = vector_i_j.x;
				}
			}

			//auf Gravitationslängeskalieren
			abstandZwischenBodys.ankatete = vector_i_j.x;
			abstandZwischenBodys.gegenkatete = vector_i_j.y;
			abstandZwischenBodys.hypotenuseBerechnen();
			geschwindigkeit.ankatete = nbodys[i].bewegungsRichtung.x;
			geschwindigkeit.gegenkatete = nbodys[i].bewegungsRichtung.y;
			geschwindigkeit.hypotenuseBerechnen();
			if (abstandZwischenBodys.hypotenuse != 0) {
				stauchungsVerhaeltnis = float(geschwindigkeit.hypotenuse) / float(abstandZwischenBodys.hypotenuse);
			} else {
				stauchungsVerhaeltnis = 1;
			}
			Geschwindigkeitsrichtung.x = Geschwindigkeitsrichtung.x*stauchungsVerhaeltnis;
			Geschwindigkeitsrichtung.y = Geschwindigkeitsrichtung.y*stauchungsVerhaeltnis;
			nbodys[i].bewegungsRichtung = Geschwindigkeitsrichtung;

			//Eigene Geschwindigkeit bewegen
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
	RECT rect = { 0,0,1000,1000 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);
	HWND windowshandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MainWindow", L"DirectX", WS_OVERLAPPEDWINDOW, -1300, 0,
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



