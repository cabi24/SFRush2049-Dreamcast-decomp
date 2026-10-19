import pathlib,json
p=pathlib.Path('matching-evidence/2026-10-18/campaign100-lane5')
tasks=[('stree','DecodeSlist'),('stree','DecodeVlist'),('stunt','_AddStunt'),('stunt','_CheckMulti'),('unk_8C055F38','range reduction'),('unk_8C055F38','second distinct target unavailable'),('targetpaths','_PlacePathTargetQuat'),('targetpaths','_StartPathTarget'),('targets','SetupTarget'),('targets','_LocalPointInBody'),('TB_text','TB_Strcpy'),('TB_text','TB_Printf'),('videoscr','initScreen'),('videoscr','uninitScreen'),('visuals','InitCarDynamicObjs'),('visuals','InitCarVisuals')]
rows=[dict(id=i+1,unit=u,target=t,status='pending',attempts=0,delta_functions=0,delta_text=0,residual='') for i,(u,t) in enumerate(tasks)]
updates=[(0,1,'mask-before-subtract fixes scheduling;1 remaining pool displacement; discard no strict gain'),(1,2,'complement maskFF1F and assigned return yield2 pool displacement bytes; downstream VectorToSurfEdge pool2 bytes early, no strict gain'),(2,1,'equivalent positive comparison unchanged25 register-allocation bytes'),(3,1,'equivalent sum grouping unchanged219 bytes; entry shifted8 by earlier CheckFlip and inner pool shifts differ')]
for i,n,r in updates:rows[i].update(status='bounded blocker; restored',attempts=n,residual=r)
(p/'tasks.json').write_text(json.dumps(rows,indent=2))
