%token CHAR
%token LBRACK RBRACK
%token LPAREN RPAREN

%%

pattern : leading_anchor fragments trailing_anchor
		;

leading_anchor : '^'
			   |
			   ;

trailing_anchor : '$'
				|
				;

fragments : fragments fragment
		  | fragment
		  ;

fragment : token modifier
		 ;

modifier : '*'
		 | '+'
		 | '{' num_range '}'
		 |
		 ;

num_range : NUM
		  | NUM ','
		  | ',' NUM
		  | NUM ',' NUM
		  ;

token : sequence_expr
	  | class
	  | group
	  ;

class : '[' '^' classdesc ']'
	  | '[' classdesc ']'
	  ;

classdesc : class_sequence
		  | ':' class_name ':'
		  | CHAR '-' CHAR
		  | 
		  ;

class_name : "alnum"
		   | "alpha"
		   | "blank"
		   | "cntrl"
		   | "digit"
		   | "graph"
		   | "lower"
		   | "print"
		   | "punct"
		   | "space"
		   | "upper"
		   | "xdigit"
		   | ne_alpha_sequence
		   ;

class_sequence : class_sequence CHAR
			   | class_sequence '[' '.' sequence '.' ']' /* collating elements */
			   | class_sequence '[' '=' sequence '=' ']' /* equivalence classes */
			   |
			   ;

group : '(' sequence_expr ')'
	  ;

sequence_expr : sequence_expr | sequence
			  ;

ne_alpha_sequence : alpha_sequence CHAR
				  | CHAR
				  ;

sequence : sequence CHAR
		 | CHAR
		 | '.'
		 |
		 ;

