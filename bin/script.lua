-- LUA Test script for NodeBEAM Editor
-- Chassis generator

--Settings
length = 6.0
width = 1
height = 0.5
segments = 15

--Generator itself
step = length/segments
offx = width/2
offy = -length/2
offz = width/2

--Place nodes
counter=0
for i = 0, segments, 1 do
	NodeName = "cha"..counter
	NB:AddNode(NodeName , offx, offy, offz)
	NodeName = "cha"..counter+1
	NB:AddNode(NodeName , -offx, offy, offz)
	NodeName = "cha"..counter+2
	NB:AddNode(NodeName , offx, offy, -offz)
	NodeName = "cha"..counter+3
	NB:AddNode(NodeName , -offx, offy, -offz)
	counter = counter + 4
	offy = offy + step
end

--Place beams
counter=0
for i = 1, segments, 1 do
	BeamNode1 =  "cha"..counter
	BeamNode2 =  "cha"..(counter+4)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	BeamNode1 =  "cha"..counter+1
	BeamNode2 =  "cha"..(counter+5)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  "cha"..counter+2
	BeamNode2 =  "cha"..(counter+6)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  "cha"..counter+3
	BeamNode2 =  "cha"..(counter+7)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	counter = counter + 4
end

--More beams
counter=0
for i = 0, segments, 1 do
	BeamNode1 =  "cha"..counter
	BeamNode2 =  "cha"..(counter+1)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	BeamNode1 =  "cha"..(counter+1)
	BeamNode2 =  "cha"..(counter+3)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  "cha"..(counter+2)
	BeamNode2 =  "cha"..(counter+3)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  "cha"..(counter+2)
	BeamNode2 =  "cha"..counter
	NB:AddBeam(BeamNode1, BeamNode2)
	
	counter = counter + 4
end
