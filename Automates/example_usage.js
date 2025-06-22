import {Parser} from "nearley";
import grammar from "./grammar.js";

const parser = new Parser(grammar);
parser.feed("hello Alice");
console.log(parser.results); // [["hello","Alice"]]
