%{
#include <stdio.h>
#include <string>

#include "parser.tab.hpp"

using namespace std;

int yylex();
%}


%x COMMENT

number [0-9]+
pid [_a-z]+
blank [\n\t \r]*
punctuation [\(\)\:\;\,]

%%
"DECLARE"           {return DECLARE;}
"BEGIN"             {return MY_BEGIN;}
"END"               {return END;}

"IF"                {return IF;}
"THEN"              {return THEN;}
"ELSE"              {return ELSE;}
"ENDIF"             {return ENDIF;}

"WHILE"             {return WHILE;}
"DO"                {return DO;}
"ENDWHILE"          {return ENDWHILE;}

"REPEAT"            {return REPEAT;}
"UNTIL"             {return UNTIL;}

"FOR"               {return FOR;}
"FROM"              {return FROM;}
"TO"                {return TO;}
"DOWNTO"            {return DOWNTO;}
"ENDFOR"            {return ENDFOR;}

"WRITE"             {return WRITE;}
"READ"             {return READ;}

":="                {return IS;}

"="                 {return EQ;}
"<"                 {return LT;}
">"                 {return GT;}
"<="                {return EQLT;}
">="                {return EQGT;}

"+"                  {return ADD;}
"-"                  {return SUB;}
"*"                  {return MUL;}
"/"                  {return DIV;}
"%"                 {return MOD;}

{pid}               {yylval.pid = new string(yytext); return pidentifier;}
{number}            {yylval.num = std::stoll(yytext); return num;}
{punctuation}        {return yytext[0];}

"["                 {BEGIN(COMMENT);}
<COMMENT>{
    "["                 ;
    "]"                 BEGIN(INITIAL);
    .                   ;
}
{blank}             {}
.                   ;
%%


int yywrap(){
    return 1;
}