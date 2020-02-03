#!/usr/bin/php
<?php
    class Server
    {
        private $socket;
        private $serverIp = "94.140.114.192";
        private $serverPort = 667;

        public function __construct()
        {
            self::createSocket();
            self::bindSocket();
            self::listenSocket();
        }

        private function listenSocket()
        {
            while (1) {
                socket_recvfrom($this->socket, $buf, 4096, 0, $from, $port);
                echo $buf;
            }
        }

        private function bindSocket()
        {
            if (!socket_bind($this->socket, $this->serverIp, $this->serverPort)) {
                $errorCode = socket_last_error();
                $errorMsg = socket_strerror($errorCode);
                die("socket_create: [$errorCode] $errorMsg.\n");
            }
            return;
        }

        private function createSocket()
        {
            $this->socket = socket_create(AF_INET, SOCK_DGRAM, 0);
            if ($this->socket < 0) {
                $errorCode = socket_last_error();
                $errorMsg = socket_strerror($errorCode);
                die("socket_create: [$errorCode] $errorMsg.\n");
            }
            return;
        }
    }
    $test = new Server;
?>