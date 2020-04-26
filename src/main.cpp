//Roberto Menegais 201810233
//
//Instruções para o uso do programa:
//
//O Programa por padrão carregará um arquivo figuras.gr se estiver presente. Para interagir com o programa, todos os comando são feitos através de atalhos do teclado,
//para manter a interface simples e limpa, os atalhos são:
//
//i - Entra no modo de inserção de figura, onde com as teclas de 0-9 pode se escolher o tipo de figura a ser inserida:
//     Modo de Inserção:
//        0 - Poligono livre: Pontos são inseridos na posição que o mouse é clicado
//        1 - Circulo: Circulo é inserido na posição do centro do mouse
//        2 - Nenhum: Não existe poligono
//        3 - Triangulo: Triangulo é inserido na posição do centro do mouse
//        4 - Quadrado: Quadrado é inserido na posição do centro do mouse
//        5 - Pentagono: Pentagono é inserido na posição do centro do mouse
//        6 - Hexagono: Hexagono é inserido na posição do centro do mouse
//        7 - Heptagono: Heptagono é inserido na posição do centro do mouse
//        8 - Octagono: Octagono é inserido na posição do centro do mouse
//        9 - Eneagono: Nonagono é inserido na posição do centro do mouse
//        Enter - Insere a figura e sai do modo de inserção
//        i - Limpa os vértices inseridos
//t - Entra no modo de translação, se houver alguma figura selecionada:
//      Modo de Translação:
//        Mover o mouse: Move as figuras selecionadas, de acordo com o centro da seleção
//        x - Fixa o eixo X, figura só é movida nesse eixo
//        y - Fixa o eixo Y, figura só é movida nesse eixo
//        t - Sai do modo de translação
//        r - Entra no modo de rotação, se houver alguma figura selecionada:
//Modo de Rotação:
//Mover o mouse: Rotaciona as figuras selecionadas, de acordo com o centro da seleção
//r - Sai do modo de rotação
//s - Entra no modo de reescala, se houver alguma figura selecionada:
//      Modo de Reescala:
//        Mover o mouse: Escala as figuras selecionadas, de acordo com o centro da seleção
//        x - Fixa o eixo X, figura só é escalada nesse eixo
//        y - Fixa o eixo Y, figura só é escalada nesse eixo
//        s - Sai do modo de reescala
//        b - Mostra as bounding box das figuras
//z - Mostra o indice da figura, que define em qual ordem a mesma será renderizada,
//    se o ZIndex de duas figuras são iguais, elas podem ou não ser reordenadas na lista, portanto é necessário que os indices sejam diferentes para garantir que as figuras
//    fiquem na ordem esperada
//left arrow - Move a figura para trás, diminuindo o indice por 10
//right arrow - Move a figura para frente, aumentando o indice por 10
//o - Carrega o arquivo figuras.gr, se existir
//p - Salva o estado da cena atual no arquivo figuras.gr
//l - Abre o color picker e controla a cor das linhas das figuras selecionadas
//f - Abre o color picker e controla a cor do preenchimento das figuras selecionadas
//g - Seta as figuras selecionadas com somente preenchimento, sem a borda
//h - Seta as figuras selecionadas com somente borda, sem preenchimento
//j - Seta as figuras selecionadas com borda e preenchimento
//ctrl - Enquanto ficar pressionada, permite que multiplas figuras sejam selecionadas ao clicar em cima das mesmas
//clique do mouse esquerdo - Seleciona figura
//del - Deleta figuras selecionadas
//
//Funcoes:
//
//Inserir figuras
//Deletar figuras
//Selecionar cor da linha da figura
//Selecionar cor do preenchimento da figura
//Mover figuras pelo mouse
//Reescalar figuras pelo mouse
//Rotacionar figuras pelo mouse
//Mover figuras para trás
//Mover figuras para frente
//Selecionar multiplas figuras
//Salvar em arquivo
//Carregar de arquivo
//Geração de Color picker
//Uso do Color picker e color slider
//Janela flutuante com possibilidade de mover com o mouse
//Inserção de poligonos arbitrários

/// Main class of the program, used for registering the callbacks
/// in GLUT, initialize the window, call the GlobalManager functions
/// and instantiate the program main manager

#include "Canvas/gl_canvas2d.h"
#include "Utilities.h"
#include "Managers/GlobalManager.h"
#include "Scene/Scene.h"

using namespace std;

void render() {
    GlobalManager::getInstance()->render();
}

void keyboard(int key) {
    GlobalManager::getInstance()->keyboard(key);
}

void keyboardUp(int key) {
    GlobalManager::getInstance()->keyboardUp(key);
}

void mouse(int button, int state, int wheel, int direction, int x, int y) {
    GlobalManager::getInstance()->mouse(button, state, wheel, direction, x, y);
}

int main(void) {
    initCanvas(GlobalManager::getInstance()->screenWidth, GlobalManager::getInstance()->screenHeight, "VectorFigures");
    Scene *scene = new Scene();
    runCanvas();
}
