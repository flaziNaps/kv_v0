测试需求
1、set a 1之后我们进行get a应该得到1，get一个不存在得键应该提示没有找到，空输入应该不崩，set得key重复之后应该有一个覆盖写
2、del key 应该删除这个k/v,如果删除一个不存在得key应该提示
3、参数的数量验证的测试，参数数量不对时要提示
4、showAll命令展示所有数据
5、quit命令要求输入后退出程序
6、contains命令要求指定key要能找到

补充（补漏的用例）：
7、get 一个已被 del 的 key，应提示没有找到（验证 del 真的删掉了）
8、del 同一个 key 两次，第二次应提示失败
9、未知命令应提示，且不崩溃
10、contains 一个不存在的 key 应返回 false
11、showAll 在空存储时应有提示（No data in the store.）
12、quit 之后，后续命令不应再被执行

已知限制（暂不实现）：set 的 value 若含空格，只会取到第一个词；
若要支持 "set a hello world" 这种带空格的值，需要给解析器加引号规则。