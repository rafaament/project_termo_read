Projeto Medidor de temperatura do termopar e pico

Descrição:
-O sistema consiste em realizar a leitura do termopar e da raspberry pico e mostrar no display a temperatura atual;
-No display, a primeira informação é o T/C que representa a temperatura do termopar em unidades C (Celsius) ou F (Fahrenheit); 
-A informação CPU, abaixo do T/C representa a temperatura da raspberry pico, em unidades C (Celsius) ou F (Fahrenheit);
-Na direita do display, temos as informações C e S. C representa a amostra atual que é coletada e S a quantidade total de amostras que serão coletadas;
-A cada X amostras da temperatura (C) capturadas, o display atualiza a informação da temperatura atual baseado na média dessas 30 amostras coletadas (S);
-Ao apertar o botão, o sistema troca a unidade de temperatura (Celsius para Fahrenheit ou vice-versa), e reseta a contagem de amostras;
-O potênciometro presente também no sistema, serve para regular a quantidade de amostras que serão coletadas. Ao girar sentido horário, aumentarará o S, e anti-horário, diminuirá.


