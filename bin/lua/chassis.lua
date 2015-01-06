-- LUA Example script for NodeBEAM Editor
-- Chassis generator

--Settings
length = 5.0
width = 0.8
height = 0.4 
segments = 6

nameprefix = "cha"

--Calculating values
step = length/segments
offx = width/2
offy = -length/2
offz = height/2

--Place nodes
counter=0
NB:AddComment("Chassis nodes")
for i = 0, segments, 1 do
	NodeName = nameprefix..counter
	NB:AddNode(NodeName , offx, offy, offz)
	NodeName = nameprefix..counter+1
	NB:AddNode(NodeName , -offx, offy, offz)
	NodeName = nameprefix..counter+2
	NB:AddNode(NodeName , offx, offy, -offz)
	NodeName = nameprefix..counter+3
	NB:AddNode(NodeName , -offx, offy, -offz)
	counter = counter + 4
	offy = offy + step
end

NB:AddComment("Chassis beams")
NB:AddComment("BNEcolor:125,135,100")
--Place beams
counter=0
for i = 1, segments, 1 do
	BeamNode1 =  nameprefix..counter
	BeamNode2 =  nameprefix..(counter+4)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	BeamNode1 =  nameprefix..counter+1
	BeamNode2 =  nameprefix..(counter+5)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  nameprefix..counter+2
	BeamNode2 =  nameprefix..(counter+6)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  nameprefix..counter+3
	BeamNode2 =  nameprefix..(counter+7)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	BeamNode1 =  nameprefix..counter
	BeamNode2 =  nameprefix..(counter+6)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	BeamNode1 =  nameprefix..(counter+1)
	BeamNode2 =  nameprefix..(counter+7)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  nameprefix..(counter+2)
	BeamNode2 =  nameprefix..(counter+4)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  nameprefix..(counter+3)
	BeamNode2 =  nameprefix..(counter+5)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	counter = counter + 4
end

NB:AddComment("Chassis beams-side")
NB:AddComment("BNEcolor:100,100,100")
counter=0
for i = 0, segments, 1 do
	BeamNode1 =  nameprefix..counter
	BeamNode2 =  nameprefix..(counter+2)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	BeamNode1 =  nameprefix..counter+1
	BeamNode2 =  nameprefix..(counter+3)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	counter = counter + 4
end

--Comment different type of beams
--and set color
NB:AddComment("Chassis beams-straight")
NB:AddComment("BNEcolor:65,135,10")
counter=0
for i = 0, segments, 1 do
	BeamNode1 =  nameprefix..counter
	BeamNode2 =  nameprefix..(counter+1)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	BeamNode1 =  nameprefix..(counter+1)
	BeamNode2 =  nameprefix..(counter+2)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  nameprefix..(counter+2)
	BeamNode2 =  nameprefix..(counter+3)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  nameprefix..(counter+3)
	BeamNode2 =  nameprefix..counter
	NB:AddBeam(BeamNode1, BeamNode2)
	
	counter = counter + 4
end

NB:AddComment("Chassis beams-X")
NB:AddComment("BNEcolor:20,100,175")
counter=0
for i = 1, segments, 1 do
	BeamNode1 =  nameprefix..counter
	BeamNode2 =  nameprefix..(counter+5)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	BeamNode1 =  nameprefix..(counter+1)
	BeamNode2 =  nameprefix..(counter+4)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  nameprefix..(counter+2)
	BeamNode2 =  nameprefix..(counter+7)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  nameprefix..(counter+3)
	BeamNode2 =  nameprefix..(counter+6)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	counter = counter + 4
end

NB:AddComment("Chassis beams-middle")
NB:AddComment("BNEcolor:175,100,10")
counter=0
for i = 1, segments, 1 do
	BeamNode1 =  nameprefix..counter
	BeamNode2 =  nameprefix..(counter+7)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	BeamNode1 =  nameprefix..(counter+1)
	BeamNode2 =  nameprefix..(counter+6)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  nameprefix..(counter+2)
	BeamNode2 =  nameprefix..(counter+5)
	NB:AddBeam(BeamNode1, BeamNode2)

	BeamNode1 =  nameprefix..(counter+3)
	BeamNode2 =  nameprefix..(counter+4)
	NB:AddBeam(BeamNode1, BeamNode2)
	
	counter = counter + 4
end

