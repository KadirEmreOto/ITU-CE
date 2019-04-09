
rm /dev/queue0
rm /dev/queue1
rm /dev/queue2
rm /dev/queue3
rm /dev/queue11

rmmod queue
make
insmod ./queue.ko queue_nr_devs=10

mknod /dev/queue0 c 246 0
mknod /dev/queue1 c 246 1
mknod /dev/queue2 c 246 2
mknod /dev/queue3 c 246 3
mknod /dev/queue11 c 246 11

echo keo > /dev/queue1
echo ------
echo queue1
echo ------
cat /dev/queue1


echo burak > /dev/queue1
echo ------
echo queue1
echo ------
cat /dev/queue1

echo ezgi > /dev/queue2
echo ------
echo queue2
echo ------
cat /dev/queue2
echo ------

echo hata > /dev/queue11

