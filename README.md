# Processamento Gráfico
### Trabalho de Grau A

## Descrição
O jogo criado deve ser jogado por dois jogadores: o atirador (anjo) e o alvo (demônio voador). O objetivo é que o anjo atire flechas no demônio. Cada flecha que acertar o alvo soma 1 ponto para o anjo. O demônio não acumula pontos, então é impossível que ele vença contra o bem. Uma pena para o jogador que escolher este personagem. A pontuação é exibida no console.

## Controles
Demônio:
  - W: Desloca para cima
  - S: Desloca para baixo
 
Anjo:
  - O: Desloca para cima
  - L: Desloca para baixo
  - SPACE: Atira flecha
  
## Detalhes da implementação
* Jogo para dois jogadores
* Movimentação
* Colisão
* Flecha: apenas uma pode estar em jogo por vez
* Todos objetos com texturas
* Anjo, demônio e flecha animados
* O cenário implementa parallax, e ainda utiliza o posicionamento de ambos jogadores para se deslocar de acordo
* O código foi extensivamente modificado para que todas variáveis estejam bem identificadas e as funções dividam as responsabilidades do ciclo de vida do jogo com clareza
* Infelizmente diversas variáveis globais são utilizadas, por falta da classe GameObject, que será apresentada no Grau B

## Execução
O projeto é uma modificação extensiva dos recursos disponibilizados pela professora no projeto ```Hello Textures - STB - Mexido```.
Deve ser utilizado o Visual Studio 2019, assim como a Windows SDK10.

Para executar, o arquivo da solução(.sln) deve ser carregado com o VS2019, e executado no modo x86.
