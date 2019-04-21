

function main()
	local all=0
	local t=os.clock()
	for i=1,100000000 do
	 all=all+i
	end
	print(os.clock()-t,all)
end

main()