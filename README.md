**Preklad servera a clienta v domovskom adresári projektu:**
```
make
```

**Odporúčaný command pred prekladom:**
```
make clean
```

**Štart serveru:**
```
./isaserver -p <port>
```

**Štart klienta:**
```
./isaclient -H <host> -p <port> <command>
```
Klient po získaní odpovede vypíše _status code_, _status message_ a _body_ odpovede.

**Obmädzenia:**
Server prijíma requesty s maximálnou dĺžkou buffru BUFSIZ.
Server nepodporuje Transfer-Encoding: chunked.
Server po obslúžení socket uzavrie ( Connection : keep-alive nie je podporované ).
Metódy POST a PUT vyžadujú Content-Length väčšie ako 0, inak server vráti error.