# rust_compiler

# 12月3日
上传了一个lexer demo 
因为是用xcode写的 windows没办法直接运行 把main.cpp代码复制粘贴一下就可以了
（可以把自己lexer也传上来大家互相学习一下 我c++真的好久没用了。。忘的差不多了

语法分析器好像要先要把上下文无关文法构造出来
这个还是挺复杂的，就比如 

`statements => statement+statemens ｜ null`

`statement  => if......`
 
`statement  => while......`
            
      
这样感觉有好多 我感觉可以先不把所有rust语法都实现。。有的太复杂了，就先把基本的语法实现
构造完了之后感觉就是下手一个个去写了
