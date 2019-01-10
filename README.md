# MaxClique
Heuristic solution for solving max clique problem.

### Note

核心代码

- `remove`：将和节点v相关的所有边e移出L
- `insert`：将和节点v相关的未被覆盖边e插入到L的表头
  - 未被覆盖边：e的另一顶点u不能在部分点覆盖C中
- `resize`：**TODO** 将点覆盖C的大小缩小至`ub-delta`，方法是依次移除dscore最高的顶点，将这些顶点的关联的未覆盖边插入到L中
- `greedyVC`：将部分点覆盖C扩展成点覆盖，方法是依次加入关联L中未覆盖边数目最多的顶点
- `chooseSwapPair`：**TODO** 依照论文实现
- `EWLS`：**两处TODO** EWLS算法实现，分为init部分和迭代部分

### Road Map

- [ ] 4处**TODO**的修改，按照论文中的说明需要random选择元素
- [ ] 实现EWLS的升级版——EWCC算法
- [ ] 对代码实现进行性能调优（施工中）
- [ ] 调参工程：选择合适的`delta`和`maxSteps`

### Reference

[Cai, Shaowei, Kaile Su, and Abdul Sattar. "Local search with edge weighting and configuration checking heuristics for minimum vertex cover." *Artificial Intelligence* 175.9-10 (2011): 1672-1696.](https://www.sciencedirect.com/science/article/pii/S0004370211000427)