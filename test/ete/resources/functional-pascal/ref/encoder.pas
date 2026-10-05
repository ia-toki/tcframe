unit encoder;

interface

function encode(x: longint): longint;

implementation

function encode(x: longint): longint;
begin
  encode := x * 2;
end;

end.
