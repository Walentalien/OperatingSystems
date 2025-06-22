@lexer lexer

@{%
const moo = require("moo");

const baseLexer = moo.compile({
  ws:    { match: /[ \t\n\r]+/, lineBreaks: true },
  true:  "true",
  false: "false",
  and:   "and",
  or:    "or",
});

const lexer = {
  reset(chunk, info) {
    this.lexer = baseLexer.reset(chunk, info);
  },
  next() {
    let token;
    while ((token = this.lexer.next())) {
      if (token.type !== "ws") return token; // skip whitespace
    }
    return token;
  },
  save() {
    return this.lexer.save();
  },
  formatError(token, message) {
    return baseLexer.formatError(token, message);
  },
  has(tokenType) {
    return true;
  }
};

module.exports.lexer = lexer;
%}

S -> E
E -> D | E "or" D
D -> C | D "and" C
C -> "true" | "false"
