if [ ! -e ../../../../../SDK/Prebuild ]; then
	mkdir ../../../../../SDK/Prebuild
fi

if [ ! -e ../../../../../SDK/Prebuild/Include ]; then
	mkdir ../../../../../SDK/Prebuild/Include
fi

if [ ! -e ../../../../../SDK/Prebuild/Linux ]; then
	mkdir ../../../../../SDK/Prebuild/Linux
fi

if [ ! -e ../../../../../SDK/Prebuild/Linux/amd64 ]; then
	mkdir ../../../../../SDK/Prebuild/Linux/amd64
fi

if [ ! -e ../../../../../SDK/Prebuild/Linux/amd64/$1 ]; then
	mkdir ../../../../../SDK/Prebuild/Linux/amd64/$1
fi

if [ -e ../../../../../Externals/$2/$2 ]; then
	if [ ! -e ../../../../../SDK/Prebuild/Include/$2 ]; then
		mkdir ../../../../../SDK/Prebuild/Include/$2
	fi
	cp -uv ../../../../../Externals/$2/$2/*.h ../../../../../SDK/Prebuild/Include/$2
fi

cp -uv ../../../../../Externals/$2/*.h ../../../../../SDK/Prebuild/Include
cp -uv *.a ../../../../../SDK/Prebuild/Linux/amd64/$1

