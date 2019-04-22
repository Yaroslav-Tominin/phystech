# GCN - нейронные сети на графах.
## Материалы не включенные в статью
### Было проведено исследование на dataset Zachary’s Karate Club и улучшена работа алгортима, взятого с [сайта](https://towardsdatascience.com/how-to-do-deep-learning-on-graphs-with-graph-convolutional-networks-62acf5b143d0).
## Концепция эксперимента:
- Попробовать применить различные нелинейности на слоях
- Поменять количество слоев и количество признаков на каждом слое
- Добавить к входным данным новые признаки вершин 
### Все результаты эксперимента можно найти [здесь](https://github.com/Yaroslav-Tominin/phystech/blob/master/ml/IPPI_experiments_with_layers.pdf)
#### Хочется обратить внимание на два результата
##### Изначально код в среднем правильно 23 человека из 32
![GitHub Log](2.png)
##### У нас получилось добиться среднего значения 28.8!
![GitHub Lo](1.png)
### Данная гифка показывает, как раздвигаются вершины в двухмерном пространстве признаков, в том случае, когда наш метод правильно класифицировал 31 человека из 32
![GitHub Logo](IPPI_gif_my_solution.gif)

### Код можно посмотреть [здесь](https://github.com/Yaroslav-Tominin/phystech/blob/master/ml/IPPI_notebook_karate_club.py)


- IPPI_experiments_with_layers.pdf - таблица с результатами эксперимента
- IPPI_presentation - презентация для ИППИ
- IPPI_code - код на jupiter
- slides_final -презентация по HOG
- TomininY2018Project25_31.pdf -статья по HOG
- HOG_inplace_final-Copy1.ipynb - код по hog











