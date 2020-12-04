//
//  lexer.cpp
//
//
//  Created by clement on 2020/12/3.
//

#include <cstdio>
#include <string>
#include <iostream>
using namespace std;
char program[1000]; //存储数组
char currentToken, nextToken;
char token[1000];
char filename[30];
FILE *fpin;

int tokenType, p = 0, n, line = 1;


const char *keyword[53]={"as","async","await","break","const","continue","crate","dyn","else","enum","extern","false","fn","for","if","impl","in","let","loop","match","mod","move","mut","pub","ref","return","self","Self","static","struct","super","trait","true","type","unsafe","use","where","while","i8","i16","i32","i64","f8","f16","f32","f64","u8","u16","u32","u64","bool","char","main"};

void getToken();

int main()
{
    p=0;
     cout<<"请输入源文件名:";
     for(;;)
     {
         cin>>filename;
         if((fpin=fopen(filename,"r"))!=NULL)//只读
        {
            break;
        }
        else
        {
            cout<<"文件输入错误！请输入源文件名：";
        }
    }
    //将fpin中的所有字符通过currentToken传入字符数组program[]中
    do
    {
         currentToken=fgetc(fpin);
         program[p++]=currentToken;
    }while(currentToken!=EOF);
    p=0;
    do
    {
        getToken();//启动字符识别函数
        if(currentToken==EOF) break;
        switch(tokenType)//打印字符状态
        {
            case 1:cout<<"("<<line<<" "<<token<<" "<<"标识符"<<")"<<endl;break;
            case 2:cout<<"("<<line<<" "<<token<<" "<<"保留字"<<")"<<endl;break;
            case 3:cout<<"("<<line<<" "<<token<<" "<<"整型"<<")"<<endl;break;
            case 31:cout<<"("<<line<<" "<<token<<" "<<"浮点型"<<")"<<endl;break;
            case 4:cout<<"("<<line<<" "<<token<<" "<<"特殊符号"<<")"<<endl;break;
            case -1:cout<<"("<<line<<" "<<"错误！"<<")"<<endl;break;
            default:break;
        }
     }while(currentToken!=EOF);
     return 0;
}


void getToken() //
{
    tokenType = 0;
    //把token[]数组清空
    for (n=0; n<1000; n++) {
        token[n]='\0';
    }
    n=0;
    currentToken = program[p];
    p+=1;
    nextToken = program[p];
    while(currentToken == ' '|| currentToken == '\t'){
        currentToken = program[p++];
        nextToken = program[p];
    }
    if(currentToken == '\n'){
        line++;
        return;
    }
    if((currentToken>='a'&&currentToken<='z')||(currentToken>='A'&&currentToken<='Z')||(currentToken=='_'))
        {
            //标识符 状态1
            tokenType=1;
            do{
                token[n++]=currentToken;
                currentToken=program[p++];
            }while ((currentToken>='a'&&currentToken<='z')||(currentToken>='A'&&currentToken<='Z')||(currentToken>='0'&&currentToken<='9')||(currentToken=='_'));
            
            for(n=0;n<53;n++)//比较标识符与keyword的关键字是否相同，若相同转为类型2
            {
                if(strcmp(token,keyword[n])==0){
                    tokenType=2;
                    break;
                }
            }
            p--;
            return;
        }
    else if (currentToken>='0'&&currentToken<='9')
    {
        //识别到数字，类型为3
        tokenType=3;
        do
        {
            token[n++] = currentToken;
            currentToken = program[p++];
            if(currentToken=='.'){
                tokenType=31;//浮点数类型为31
                token[n++]=currentToken;
                currentToken=program[p++];
            }
        }while(currentToken>='0'&&currentToken<='9');
        p--;
        return;
    }
    //跳过注释的内容
    else if(currentToken=='/' && nextToken=='*')
    {
        p++;
        do{
            currentToken=program[p++];
            nextToken=program[p++];
            if(currentToken=='\n'){
                line++;
            }
        }while(currentToken!='*'||nextToken!='/');
        return;
    }
    else if((currentToken=='/')&&(nextToken=='/'))
    {
    
        do{
            currentToken=program[p++];
        }while(currentToken!='\n');
        line++;
        return;
    }
    else if(currentToken=='='&& nextToken=='='){ //特殊字符 类型4
        p++;
        tokenType=4;
        token[0]='=';
        token[1]='=';
        return;
    }
    else if(currentToken=='<'&& nextToken=='='){
        p++;
        tokenType=4;
        token[0]='<';
        token[1]='=';
        return;
    }
    else if(currentToken=='>'&& nextToken=='='){
        p++;
        tokenType=4;
        token[0]='>';
        token[1]='=';
        return;
    }
    else if(currentToken=='!'&& nextToken=='='){
        p++;
        tokenType=4;
        token[0]='!';
        token[1]='=';
        return;
    }
    else if(currentToken=='&'&& nextToken=='&'){
        p++;
        tokenType=4;
        token[0]='&';
        token[1]='&';
        return;
    }
    else if(currentToken=='|'&& nextToken=='|'){
        p++;
        tokenType=4;
        token[0]='|';
        token[1]='|';
        return;
    }
    else
    {
        switch(currentToken)//识别特殊符号
        {
            case '&':
            case '.':
            case '=':
            case '<':
            case '>':
            case '/':
            case '+':
            case '-':
            case '*':
            case '{':
            case '}':
            case ';':
            case '(':
            case ')':
            case ',':
            case '\\':
            case '\'':
            case '\"':tokenType=4;token[0]=currentToken;break;
            default:tokenType=-1;break;
        }
    }
    return;
 }
    
    





