-- LUA Example script for NodeBEAM Editor
-- Create live axle

--Default values
length = 0.5
width = 2.0
height = 0.5 
hubwidth = 0.25
nameprefix = "axl"
steering = false

function create_axle()
	
	local offx = width/2
	--Place nodes
	counter=0
	NB:AddComment("Axle nodes "..nameprefix)
	NB:AddNode(nameprefix.."0" , 0, 0, 0)
	NB:AddNode(nameprefix.."1" , 0, -length, 0)
	NB:AddNode(nameprefix.."2" , 0, 0, height)
	NB:AddNode(nameprefix.."hr0" , -offx, 0, 0)
	NB:AddNode(nameprefix.."hl0" , offx, 0, 0)
	NB:AddNode(nameprefix.."3" , -offx, -length, 0)
	NB:AddNode(nameprefix.."4" , offx, -length, 0)
	NB:AddNode(nameprefix.."5" , -offx, 0, height)
	NB:AddNode(nameprefix.."6" , offx, 0, height)
	NB:AddComment("WheelHubs")
	NB:AddNode(nameprefix.."hr1" , -offx-hubwidth, 0, 0)
	NB:AddNode(nameprefix.."hl1" , offx+hubwidth, 0, 0)
	
	if steering == true then
		NB:AddNode(nameprefix.."str" , -offx, length, 0)
		NB:AddNode(nameprefix.."stl" , offx, length, 0)
	end
	
	--Place beams
	NB:AddComment("Axle beams "..nameprefix)
	NB:AddBeam(nameprefix..0, nameprefix..1)
	NB:AddBeam(nameprefix..1, nameprefix..2)
	NB:AddBeam(nameprefix..0, nameprefix..2)
	NB:AddBeam(nameprefix.."hr0", nameprefix..3)
	NB:AddBeam(nameprefix..3, nameprefix..5)
	NB:AddBeam(nameprefix.."hr0", nameprefix..5)
	NB:AddBeam(nameprefix.."hl0", nameprefix..4)
	NB:AddBeam(nameprefix..4, nameprefix..6)
	NB:AddBeam(nameprefix.."hl0", nameprefix..6)
	
	NB:AddBeam(nameprefix..0, nameprefix..3)
	NB:AddBeam(nameprefix..0, nameprefix..4)
	NB:AddBeam(nameprefix..0, nameprefix..5)
	NB:AddBeam(nameprefix..0, nameprefix..6)
	NB:AddBeam(nameprefix..0, nameprefix.."hr0")
	NB:AddBeam(nameprefix..0, nameprefix.."hl0")
	NB:AddBeam(nameprefix..1, nameprefix..3)
	NB:AddBeam(nameprefix..1, nameprefix..4)
	NB:AddBeam(nameprefix..1, nameprefix..5)
	NB:AddBeam(nameprefix..1, nameprefix..6)
	NB:AddBeam(nameprefix..1, nameprefix.."hr0")
	NB:AddBeam(nameprefix..1, nameprefix.."hl0")
	NB:AddBeam(nameprefix..2, nameprefix..3)
	NB:AddBeam(nameprefix..2, nameprefix..4)
	NB:AddBeam(nameprefix..2, nameprefix..5)
	NB:AddBeam(nameprefix..2, nameprefix..6)
	NB:AddBeam(nameprefix..2, nameprefix.."hr0")
	NB:AddBeam(nameprefix..2, nameprefix.."hl0")
	
	if steering == true then
		NB:AddComment("Axle steering")
		NB:AddBeam(nameprefix.."hr0", nameprefix.."str")
		NB:AddBeam(nameprefix.."hr1", nameprefix.."str")
		NB:AddBeam(nameprefix..5, nameprefix.."str")
		NB:AddBeam(nameprefix.."hl0", nameprefix.."stl")
		NB:AddBeam(nameprefix.."hl1", nameprefix.."stl")
		NB:AddBeam(nameprefix..6, nameprefix.."stl")
		NB:AddBeam(nameprefix.."str", nameprefix.."stl")
	else
		NB:AddBeam(nameprefix.."hr1", nameprefix..3)
		NB:AddBeam(nameprefix.."hl1", nameprefix..4)
	end
	
	NB:AddComment("Axle beams right hub")
	NB:AddBeam(nameprefix.."hr0", nameprefix.."hr1")
	NB:AddBeam(nameprefix..5, nameprefix.."hr1")
	NB:AddComment("Axle beams left hub")
	NB:AddBeam(nameprefix.."hl0", nameprefix.."hl1")
	NB:AddBeam(nameprefix..6, nameprefix.."hl1")
	
end

--Get input values from user
local values = NB:InputValue("html/liveaxle.html")

--Check that input values are ok
local ok = true
if tonumber(values["width"]) == nil then
	ok = false
elseif tonumber(values["height"]) == nil then
	ok = false
elseif tonumber(values["length"]) == nil then
	ok = false	
elseif tonumber(values["hubwidth"]) == nil then
	ok = false	
end

for i, v in pairs(values) do
	if i == "prefix" then
		nameprefix = v
	elseif i == "steering" then
		if v == "on" then
			steering = true
		end
	end
end

if ok then
	--values are ok
	length = tonumber(values["length"])
	width = tonumber(values["width"])
	height = tonumber(values["height"])
	hubwidth = tonumber(values["hubwidth"])
	NB:Log("Values ok")
	create_axle()
else
	NB:Log("Invalid input values")
end