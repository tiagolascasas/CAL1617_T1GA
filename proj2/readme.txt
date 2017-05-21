Instruções de compilação:

Windows: basta compilar o projeto num compilador que reconheça a sintaxe C++11,
visto que faz uso dela (expressões lambda e std::numeric). Em Windows o programa usa
a biblioteca winsock2.h que, dependendo do compilador e/ou IDE, poderá ser necessário juntar
manualmente.

Linux: em Linux o programa usa a biblioteca curses.h, que permite a leitura de um caracter do teclado
sem echo (semelhante a getch() em Windows). Deste modo, a biblioteca pode ser adquirida usando
		sudo apt-get install libncurses5-dev
e o programa pode ser compilado da seguinte forma (neste exemplo, usando o g++):
		g++ -std=c++11 src/*.cpp -o proj2 -lncurses
		
O executável deverá estar no mesmo diretório do GraphViewerController.jar e da pasta res,
de modo a fazer uso de ambos.

O programa pode ser executado sem argumentos, usando assim os ficheiros na pasta res, ou
então pode ter os caminhos dos ficheiros passados como argumento, da seguinte forma:
		./proj2 nodes_file road_info_file road_file markets_file map_file

É ainda de notar que o programa deve ser corrido num terminal/consola (independentemente do SO),
visto que os terminais integrados dos IDEs (ex. Eclipse) podem não processar devidamente as 
chamadas getch().
