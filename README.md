# sshnax
crypto tunnel for execute a command

ГОВНО КОД по замене ssh тунеля у клиентов, которые ставят костыли в палки. 

ГОВНО ключ на 15 бит шифрованный RSA алгоритмом. Че так много спросите - так int! Команды ходят шифрованные и выполняются,если расшифруются на серверной части.

Для чего? Да поставить точку на своем ГОВНО программирование.

<img src="https://github.com/oditynet/sshnax/blob/main/sshnax.png" title="example" width="800" />

ТОДОхо: -сделал crc16 по передаче команды,но это лишнее и удалил.
-а оно надо вообще? ну подниму длину ключа до 56. всплакнем все вместе и удалю потом