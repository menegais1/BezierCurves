
Roberto Menegais 201810233

Instruções para o uso do programa:

Para interagir com o programa, alguns comandos são feitos através do teclado, porém os principais estão destacados em botões na interface,
para manter a interface simples e limpa, os atalhos são:

- i - Entra no modo de inserção de curva:
    - Modo de Inserção: 
      - Enter - Insere a curva e sai do modo de inserção
      - i - Limpa os pontos de controle inseridos
- t - Entra no modo de translação, se houver alguma curva selecionada:
    - Modo de Translação:
        - Mover o mouse: Move as curvas selecionadas, de acordo com o centro da seleção, computado pelo fecho convexo
        - x - Fixa o eixo X, figura só é movida nesse eixo
        - y - Fixa o eixo Y, figura só é movida nesse eixo
        - t - Sai do modo de translação
- r - Entra no modo de rotação, se houver alguma curva selecionada:
    - Modo de Rotação:
        - Mover o mouse: Rotaciona as curvas selecionadas, de acordo com o centro da seleção,, computado pelo fecho convexo
        - r - Sai do modo de rotação
- s - Entra no modo de reescala, se houver alguma curva selecionada:
    - Modo de Reescala:
        - Mover o mouse: Escala as curvas selecionadas, de acordo com o centro da seleção,, computado pelo fecho convexo
        - x - Fixa o eixo X, curva só é escalada nesse eixo
        - y - Fixa o eixo Y, curva só é escalada nesse eixo
        - s - Sai do modo de reescala
- b - Mostra o fecho convexo das curvas
- g - Mostra o grafo de controle das curvas
- h - Ativa ou desativa a animação das curvas
- c - Ativa ou desativa os pontos de controle, para melhor vizualização da curva criada
- q - Ativa a janela das blending functions da primeira curva selecionada, caso haja mais de uma em seleção, ou da curva selecionada
- ctrl - Enquanto ficar pressionada, permite que multiplas curvas sejam selecionadas ao clicar em cima das mesmas
clique do mouse esquerdo - Seleciona curva
- del - Deleta curvas selecionadas

Funcões:

    - Inserir curvas através da inserção dos pontos de controle, no máximo 40 pontos pelo fato da curva ser gerada usando as blending functions, causando overflow no float devido a operação de fatorial usada.
    - Vizualização do Grafo de controle
    - Animação da geração das curvas com controle de FPS
    - Grafico das blending functions
    - Mover os pontos de controle com o mouse
    - Deletar curvas - Extra
    - Mover curvas pelo mouse - Extra
    - Reescalar curvas pelo mouse - Extra
    - Rotacionar curvas pelo mouse - Extra
    - Selecionar multiplas curvas - Extra
    - Janela flutuante com possibilidade de mover com o mouse - Extra
    - Fecho convexo das curvas - Extra
    
OBS:

    - As curvas são selecionados de acordo com o fecho convexo e pelos pontos de controle, então se uma curva sobrepor a outra, pode ocorrer da curva embaixo não ser selecionada ao clicar dentro do fecho convexo dela, porém os pontos de controle são sempre clicáveis para selecionar a curva.
    - As janela das blending functions só aparecerá para a curva selecionada, se nenhuma curva estiver selecionada nada acontece.