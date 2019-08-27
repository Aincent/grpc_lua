targetdirect=./
alldirect=$(patsubst ./%,%,$(shell find -name '*' -type d)) 
alltarget=$(patsubst .%.cpp,$(targetdirect)%.o,$(shell find -name '*' -type f|grep -E '.+\.cpp$$'))
alldepend:=$(patsubst %.cpp,%.o,$(shell find -name '*' -type f|grep -E '.+\.cpp$$'))
alldepend+=/usr/local/lib/liblua.a
$(shell mkdir -p $(targetdirect))

targetoption=-D__cplusplus=201103L -DLUAINTF_LINK_LUA_COMPILED_IN_CXX=0 -DLUAINTF_LINK_LUA_COMPILED_IN_CXX=0 -I../lua-intf/ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC 
excuteoption=-L/usr/local/lib -Xlinker /usr/local/lib/liblua.a 
excutelip=-lpthread -lprotobuf -lgrpc++ -lgrpc -ldl

$(targetdirect)grpc.so : $(alldepend) 
	g++ -g $(excuteoption) $(alltarget) -shared -o  $(@) $(excutelip)

define buildtarget
$1/%.o:$1/%.cpp	
	g++ -g $$(targetoption) $$^ -c -o $$(targetdirect)$$@
endef

$(foreach v,$(alldirect),$(eval $(call buildtarget,$(v))))

define makedirect
$$(shell mkdir -p $$(targetdirect)$1)
endef

$(foreach v,$(alldirect),$(eval $(call makedirect,$(v))))

clean : 
	find -name '*.o' -type f -exec rm -rf {} \;	
	find ./ -name '*.o' -type f -exec rm -rf {} \;	
	rm -rf $(targetdirect)grpc.so
