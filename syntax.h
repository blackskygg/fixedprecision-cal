#ifndef SYNTAX_H_
#define SYNTAX_H_

#define R_DEC_DIGITS "[0-9]+"
#define R_HEX_DIGITS "[0-9a-f]+"
#define R_OCT_DIGITS "[0-7]+"
#define R_BIN_DIGITS "[0-1]+"
#define R_DOT "\\."
#define R_OPTION(_R, ...) "("  _R  __VA_ARGS__ ")?"

#define R_DEC_NUM "^" "[1-9][0-9]*" R_OPTION(R_DOT,R_DEC_DIGITS) "|0"
#define R_HEX_NUM "^(0x)" R_HEX_DIGITS R_OPTION(R_DOT,R_HEX_DIGITS)
#define R_OCT_NUM "^0" R_OCT_DIGITS R_OPTION(R_DOT,R_OCT_DIGITS)
#define R_BIN_NUM "^" R_BIN_DIGITS R_OPTION(R_DOT,R_BIN_DIGITS) "[b]"

#define R_ADD_POS "^\\+"
#define R_SUB_NEG "^-"
#define R_MUL "^\\*"
#define R_DIV "^/"
#define R_LEFT_BRACE "^\\("
#define R_RIGHT_BRACE "^\\)"
#define R_BLANK "^[[:space:]]+"

#define NR_SYNTAX 11

#endif
