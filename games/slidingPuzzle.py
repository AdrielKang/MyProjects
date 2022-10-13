import numpy as np

class Node(): #we are making a constructor to create a data structure
    def __init__(self, state, parent, depth):       #depth parameter was added just for the IDS
        self.state = state
        self.parent = parent    #parent node
        self.depth = depth      #depth of the node when traversing/creating tree
        self.up = None          #child
        self.down = None        #child
        self.left = None        #child
        self.right = None       #child

    #see if we can go up
    def go_up(self):
        rows, cols = np.where(self.state == 0)  #find indices of the empty cell
        indices = [rows, cols]                  #create an array with the indices of the empty cell
        if indices[0] == 1:                     #if we are on the first row then we can't go up.
            return False
        else:
            value_below = self.state[indices[0] + 1, indices[1]]        #value of the tile below since we go up
            updated_state = self.state.copy()                           #copy the state so that we can update it
            updated_state[indices[0], indices[1]] = value_below         #swaping empty tiles and lower tile value
            updated_state[indices[0] + 1, indices[1]] = 0
            return updated_state, value_below                           #update the state

    # see if we can go down
    def go_down(self):
        rows, cols= np.where(self.state == 0)
        indices = [rows, cols]
        if indices[0] == 0:
            return False
        else:
            value_above = self.state[indices[0] - 1, indices[1]]        #value of the tile above since we go up
            updated_state = self.state.copy()
            updated_state[indices[0], indices[1]] = value_above
            updated_state[indices[0] - 1, indices[1]] = 0
            return updated_state, value_above

    # see if moving left is valid
    def go_left(self):
        rows, cols = np.where(self.state == 0)
        indices = [rows, cols]
        if indices[1] == 2:
            return False
        else:
            value_right = self.state[indices[0], indices[1] + 1]        #value of the tile to the right since we go left
            updated_state = self.state.copy()
            updated_state[indices[0], indices[1]] = value_right
            updated_state[indices[0], indices[1] + 1] = 0
            return updated_state, value_right

    # see if moving right is valid
    def go_right(self):
        rows, cols = np.where(self.state == 0)
        indices = [rows, cols]
        if indices[1] == 0:
            return False
        else:
            value_left = self.state[indices[0], indices[1] - 1]         #value of the tile on the left since we go right
            updated_state = self.state.copy()
            updated_state[indices[0], indices[1]] = value_left
            updated_state[indices[0], indices[1] - 1] = 0
            return updated_state, value_left


    #Backtrack when the goal is found so that we can print the path
    def print_path(self):
        reversed_state = [self.state]                   #reversed list since we are backtracking
        while self.parent:
            self = self.parent
            reversed_state.append(self.state)
        # print out the path step by step
        counter = 1                                     #counter for the number of steps
        while reversed_state:
            print('step', counter)
            print(reversed_state.pop())                 #instead of reversing and then printing, we can just pop the list
            print("\n")
            counter += 1

    def BFS_and_UniformCostSearch(self, goal_state):    #The uniform cost search has a unit cost, hence it's just a BFS
        queue = [self]                                  #Queue for visited nodes
        visited = set([])                               #keep track of visited states to avoid repetition
        while queue:                                    #keep searching till goal is reached
            cur_node = queue.pop(0)                     #remove the first element of the queue
            visited.add(tuple(cur_node.state.reshape(1, 6)[0]))             #add the visited states to avoid repetition. Change to a tuple since numpy.ndarray is unhashable
            #print(type(visited))
            if np.array_equal(cur_node.state, goal_state):                  #print path when goal is found
                cur_node.print_path()

            else:                                                           #if not the goal, we try to move tiles to the empty tiles
                if cur_node.go_up():                                        #check if bottom node can go up
                    new_state, lower_value = cur_node.go_up()
                    if tuple(new_state.reshape(1, 6)[0]) not in visited:
                        cur_node.up = Node(state=new_state, parent=cur_node, depth=0)
                        queue.append(cur_node.up)                           #append will put at the end of list so we can pop tu get first element

                if cur_node.go_down():                                      #check if upper tile can go down
                    new_state, value_above = cur_node.go_down()
                    if tuple(new_state.reshape(1, 6)[0]) not in visited:    #if state not visited then we create the new children nodes
                        cur_node.down = Node(state=new_state, parent=cur_node, depth=0)
                        queue.append(cur_node.down)

                if cur_node.go_left():                                      #check if right node can go left
                    new_state, right_value = cur_node.go_left()
                    if tuple(new_state.reshape(1, 6)[0]) not in visited:
                        cur_node.left = Node(state=new_state, parent=cur_node, depth=0)
                        queue.append(cur_node.left)

                if cur_node.go_right():                                     #check if left node can go right
                    new_state, value_left = cur_node.go_right()
                    if tuple(new_state.reshape(1, 6)[0]) not in visited:
                        cur_node.right = Node(state=new_state, parent=cur_node,  depth=0)
                        queue.append(cur_node.right)





    def depth_first_search(self, goal_state):
        queue = [self]
        visited = set([])
        while queue:

            cur_node = queue.pop(0)
            visited.add(tuple(cur_node.state.reshape(1, 6)[0]))         #add visited states

            if np.array_equal(cur_node.state, goal_state):              #print if goal found
                cur_node.print_path()

            else:
                #we will make a list with the nodes that we can move so that we can tie break
                l=[]
                new_state1=self.state
                new_state2=self.state
                new_state3=self.state
                new_state4=self.state
                v_u=-1
                v_d=-1
                v_r=-1
                v_l=-1
                if cur_node.go_up():
                    new_state1, v_u1 = cur_node.go_up()
                    l.append(v_u1)
                    v_u += v_u1+1

                if cur_node.go_down():  # check if upper tile can go down
                    new_state2, v_d1 = cur_node.go_down()
                    l.append(v_d1)
                    v_d += v_d1+1



                if cur_node.go_left():  # check if right node can go left
                    new_state3, v_l1 = cur_node.go_left()
                    l.append(v_l1)
                    v_l +=v_l1+1

                if cur_node.go_right():  # check if left node can go right
                    new_state4, v_r1= cur_node.go_right()
                    l.append(v_r1)
                    v_r += v_r1+1
                l.sort(reverse=True)                                           #reverse the sort so that we can insert bigger
                y=0                                                            #number to the queue so that we pop the smaller ones first
                while y<len(l):                                                #loop through every element of the list to queue them up
                    x=l[y]
                    if x==v_u:
                        if cur_node.go_up():                                   #check if bottom node can go up & that we have
                            if tuple(new_state1.reshape(1, 6)[0]) not in visited:
                                cur_node.up = Node(state=new_state1, parent=cur_node, depth=0)
                                queue.insert(0, cur_node.up)                    #insert will put element in the front of list so we can pop to get last element
                    if x==v_d:
                        if cur_node.go_down():                                  #check if upper tile can go down
                            if tuple(new_state2.reshape(1, 6)[0]) not in visited:
                                cur_node.down = Node(state=new_state2, parent=cur_node, depth=0)
                                queue.insert(0, cur_node.down)

                    if x == v_l:
                        if cur_node.go_left():                                  #check if right node can go left
                            if tuple(new_state3.reshape(1, 6)[0]) not in visited:
                                cur_node.left = Node(state=new_state3, parent=cur_node, depth=0)
                                queue.insert(0, cur_node.left)

                    if x == v_r:
                        if cur_node.go_right():                                 #check if left node can go right
                            if tuple(new_state4.reshape(1, 6)[0]) not in visited:
                                cur_node.right = Node(state=new_state4, parent=cur_node, depth=0)
                                queue.insert(0, cur_node.right)

                    y+=1




    def iterative_deepening(self, goal_state):
        x=0                                                                         #maximum depth per "iteration"
        while True:                                                                 #loop by incrementing limit till we find a path
            for depth_limit in range(x):
                queue = [self]
                depth_queue = [0]                                                   #We need to keep track of the depth this time
                visited = set([])

                while queue:
                    cur_node = queue.pop(0)
                    cur_depth = depth_queue.pop(0)
                    visited.add(tuple(cur_node.state.reshape(1, 6)[0]))

                    if np.array_equal(cur_node.state, goal_state):
                        cur_node.print_path()
                        return True                                                 #returning something or else we would be stuck in an infinite loop
                    else:
                        if cur_depth < depth_limit:

                            if cur_node.go_up():
                                updated_state, value_bottom = cur_node.go_up()
                                if tuple(updated_state.reshape(1, 6)[0]) not in visited:
                                    cur_node.up = Node(state=updated_state, parent=cur_node,depth=cur_depth + 1)
                                    queue.insert(0, cur_node.up)
                                    depth_queue.insert(0, cur_depth + 1)
                            if cur_node.go_down():
                                updated_state, value_up = cur_node.go_down()
                                if tuple(updated_state.reshape(1, 6)[0]) not in visited:
                                    cur_node.down = Node(state=updated_state, parent=cur_node,depth=cur_depth + 1)
                                    queue.insert(0, cur_node.down)
                                    depth_queue.insert(0, cur_depth + 1)
                            if cur_node.go_left():
                                updated_state, value_right = cur_node.go_left()
                                if tuple(updated_state.reshape(1, 6)[0]) not in visited:
                                    cur_node.left = Node(state=updated_state, parent=cur_node,depth=cur_depth + 1)
                                    queue.insert(0, cur_node.left)
                                    depth_queue.insert(0, cur_depth + 1)
                            if cur_node.go_right():
                                updated_state, value_left = cur_node.go_right()
                                if tuple(updated_state.reshape(1, 6)[0]) not in visited:
                                    cur_node.right = Node(state=updated_state, parent=cur_node,depth=cur_depth + 1)
                                    queue.insert(0, cur_node.right)
                                    depth_queue.insert(0, cur_depth + 1)

            x+=1  #increment the max depth if we didn't find a path






initial_state = np.array([1,4,2,5,3,0]).reshape(2, 3)
goal_state = np.array([0,1,2,5,4,3]).reshape(2,3)
print("Initial state: ")
print (initial_state,'\n')
print("Goal state: ")
print (goal_state, "\n")
root_node = Node(state=initial_state,parent=None, depth=0)
print("Path when using BFS or uniform cost search with unit cost")
root_node.BFS_and_UniformCostSearch(goal_state)
print("\n\n")
print("Path when using DFS")
root_node.depth_first_search(goal_state)
print("\n\n")
print("Path when using IDS")
root_node.iterative_deepening(goal_state)
print("\n\n")
