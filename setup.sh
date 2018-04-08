docker build -t mattdebian .
docker run --name test -itd mattdebian /bin/bash
docker cp ./ test:/root

echo "#Launch cmd with : docker exec -it test /bin/bash"
echo "cd /root"
echo "make"
echo "./Matt_daemon"