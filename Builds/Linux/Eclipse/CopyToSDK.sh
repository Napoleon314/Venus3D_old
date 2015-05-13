if [ ! -e ../../../../../SDK/Include ]; then
	mkdir ../../../../../SDK/Include
fi

if [ ! -e ../../../../../SDK/Linux ]; then
	mkdir ../../../../../SDK/Linux
fi

if [ ! -e ../../../../../SDK/Linux/amd64 ]; then
	mkdir ../../../../../SDK/Linux/amd64
fi

if [ ! -e ../../../../../SDK/Linux/amd64/$1 ]; then
	mkdir ../../../../../SDK/Linux/amd64/$1
fi

cp -uv ../../../../../Source/$2/*.h ../../../../../SDK/Include
cp -uv ../../../../../Source/$2/*.inl ../../../../../SDK/Include
cp -uv *.a ../../../../../SDK/Linux/amd64/$1

