program grader;

uses
  encoder, decoder;

var
  n: longint;

begin
  readln(n);
  writeln(decode(encode(n)));
end.
