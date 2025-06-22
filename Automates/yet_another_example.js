// Generated automatically by nearley, version 2.20.1
// http://github.com/Hardmath123/nearley
(function () {
function id(x) { return x[0]; }

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
var grammar = {
    Lexer: lexer,
    ParserRules: [
    {"name": "S", "symbols": ["E"]},
    {"name": "E", "symbols": ["D"]},
    {"name": "E", "symbols": ["E", {"literal":"or"}, "D"]},
    {"name": "D", "symbols": ["C"]},
    {"name": "D", "symbols": ["D", {"literal":"and"}, "C"]},
    {"name": "C", "symbols": [{"literal":"true"}]},
    {"name": "C", "symbols": [{"literal":"false"}]}
]
  , ParserStart: "S"
}
if (typeof module !== 'undefined'&& typeof module.exports !== 'undefined') {
   module.exports = grammar;
} else {
   window.grammar = grammar;
}
})();
