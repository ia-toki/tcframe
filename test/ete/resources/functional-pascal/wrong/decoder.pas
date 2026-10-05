unit decoder;

interface

function decode(y: longint): longint;

implementation

function decode(y: longint): longint;
begin
  decode := y div 2 + 1;
end;

end.
