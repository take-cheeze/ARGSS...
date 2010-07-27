Output.msgbox

print "Testing Table class"

Output.console

NUM_MAX =  32767
NUM_MIN = -32768

XSIZE = 258
YSIZE = 19
ZSIZE = 8

test_table = Table.new(XSIZE, YSIZE, ZSIZE);
print( sprintf("xsize = %d;", test_table.xsize) )
print( sprintf("ysize = %d;", test_table.ysize) )
print( sprintf("zsize = %d;", test_table.zsize) )

result = Array.new(XSIZE * YSIZE * ZSIZE);

mrshSrc = Array.new(XSIZE * YSIZE * ZSIZE);

# input test
srand(NUM_MAX)
for z in 0 .. (test_table.zsize - 1)
	for y in 0 .. (test_table.ysize - 1)
		for x in 0 .. (test_table.xsize - 1)
			randNum = rand(NUM_MAX)
			test_table[x, y, z] = randNum
			result[x + XSIZE * (y + z * YSIZE)] = sprintf("[%2d, %2d, %2d] = %5d; ", x, y, z, randNum)
			result[x + XSIZE * (y + z * YSIZE)] += sprintf("[%2d, %2d, %2d] -> %5d;", x, y, z, test_table[x, y, z])

			mrshSrc[x + XSIZE * (y + z * YSIZE)] = randNum
			mrshSrc[x + XSIZE * (y + z * YSIZE)] = NUM_MAX if( mrshSrc[x + XSIZE * (y + z * YSIZE)] > NUM_MAX )
			mrshSrc[x + XSIZE * (y + z * YSIZE)] = NUM_MIN if( mrshSrc[x + XSIZE * (y + z * YSIZE)] < NUM_MIN )
		end
	end
end
result.each {|obj| print(obj + "\n")}

# marshal test
dumped = Marshal.dump(test_table)
loaded = Marshal.dump( Marshal.load(dumped) )
print "marshal result: "
print "redumping: "
print (dumped == loaded)

mrshDst = dumped.unpack("V5")
print sprintf("dim: %d; // 3", mrshDst[0])
print sprintf("xsize: %d; // %d", mrshDst[1], XSIZE)
print sprintf("ysize: %d; // %d", mrshDst[2], YSIZE)
print sprintf("zsize: %d; // %d", mrshDst[3], ZSIZE)
print sprintf("data_size: %d; // %d", mrshDst[4], XSIZE * YSIZE * ZSIZE)

mrsh_data_size = mrshDst[4]
mrshDst = dumped.slice(0, 20).unpack( sprintf("v%d", mrsh_data_size) );
mrshResult = true
mrshResult = false if(mrshDst.length != mrshSrc.length)
for i in 0 .. (mrsh_data_size - 1)
	mrshResult = false if( mrshDst[i] != mrshSrc[i] )
end
print "data compare: "
print mrshResult

# max test
for i in 0 .. 10
	input = rand(NUM_MAX*2 * 2) - NUM_MAX*2;
	test_table[0, 0, 0] = input;
	print sprintf("test_table[0, 0, 0] = %8d; test_table[0, 0, 0] -> %8d", input, test_table[0, 0, 0])
end

# infinity loop
loop do
	Graphics.update
	Input.update
end
