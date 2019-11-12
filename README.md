**Preklad servera a clienta v domovskom adresári projektu:**
```
make
```

**Štart servera:**
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
Server nepodporuje Transfer-Encoding: chunked
 