

host=u2pHl-1Uq-5DN-9rL-GZ0Ti4Uv
fname=Michael
lname=Makuch
email=mkm@muzikbrowzer.com
check=8b903931aed8c980e97d7ef3bda0a2d6

code=`./mbpw $host $fname $lname $email`
if [ "$code" != "$check" ] ; then
	echo "fail"
	exit 1
fi

host=4DQ6Bd-Cy6-4u6-AC5-19defBS4x
fname=John
lname=Doe
email=john@doe.com
check=f38bee8ea12ab6ccb12d97a038baf5d2

code=`./mbpw $host $fname $lname $email`
if [ "$code" != "$check" ] ; then
	echo "fail"
	exit 1
fi

host=1fFHSX-Evb-4ei-AgF-1FtBYUR1y
fname=Jane
lname=Doe
email=jane@doe.com
check=a2f35581d89cba716287afca17eb26bd

code=`./mbpw $host $fname $lname $email`
if [ "$code" != "$check" ] ; then
	echo "fail"
	exit 1
fi

echo "Test ok"
