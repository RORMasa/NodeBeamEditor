-- LUA Test script for NodeBEAM Editor

counter=0
for i = -5, 5, 0.5 do
	NodeName = "c"..counter
	NB:AddNode(NodeName , 0.25, 0.35*i, 0.35)
	counter = counter + 1
end