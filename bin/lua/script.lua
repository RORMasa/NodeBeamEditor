-- LUA Example script for NodeBEAM Editor

--There is currently 9 functions available, each in a class NB
-- NB:AddNode( name, location x, location y, location z)
-- NB:AddNodet( name, location x, location y, location z, tag)
-- NB:AddBeam( id1, id2 )
-- NB:AddComment( "your comment" )
-- NB:Log("your message") writes a log message on the script tab
-- NB:GetNode("node name") returns the node location x,y,z and id
-- NB:GetNodei(id) returns the node location  x,y,z and name
-- NB:GetNodeCount() returns the amount of nodes in current nodebeam
-- NB:GetNodes() returns all nodes in current nodebeam

--Exampe of using the AddNode function

NB:AddNode("node1" , 0.5, 0.3, 0.7)
NB:AddNode("node2" , -0.5, 0.3, 0.7)

-- The first line adds a node named "node1",
-- in location x 0,5 , y 0,3,  z 0,7.


--Example of using AddComment function
NB:AddComment("This is a comment.")	
--A comment will come before the next
--node or beam that you place in a script.


--Exampe of using the AddBeam function
NB:AddBeam("node1", "node2")

NB:Log("Hello")
