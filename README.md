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
Server po obdržení requestu vypíše request a na konci vždy vypíše _aktuálne nástenky_ s _počtom postov_ jednotlivých násteniek.<br/>

**Štart klienta:**
```
./isaclient -H <host> -p <port> <command>
```
Klient po získaní odpovede vypíše _status code_, _status message_ a _body_ odpovede.

**Obmädzenia:**<br/>
Server prijíma requesty s maximálnou dĺžkou buffru BUFSIZ.<br/>
Server nepodporuje Transfer-Encoding: chunked.<br/>
Server po obslúžení socket uzavrie (Connection : keep-alive nie je podporované).<br/>
Metódy POST a PUT vyžadujú Content-Length väčšie ako 0, inak server vráti error.<br/>
