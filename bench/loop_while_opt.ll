; ModuleID = 'bench/loop_while.ll'
source_filename = "bench/loop_while_to_cgra.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z10loop_whilePii(ptr noundef %a, i32 noundef %n) #0 {
entry:
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %n.addr.0 = phi i32 [ %n, %entry ], [ %dec, %while.body ]
  %sum.0 = phi i32 [ 0, %entry ], [ %add, %while.body ]
  %cmp = icmp sgt i32 %n.addr.0, 0
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %add = add nsw i32 %sum.0, %n.addr.0
  %dec = add nsw i32 %n.addr.0, -1
  br label %while.cond, !llvm.loop !6

while.end:                                        ; preds = %while.cond
  ret i32 %sum.0
}

attributes #0 = { mustprogress noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 16.0.1 (https://github.com/llvm/llvm-project cd89023f797900e4492da58b7bed36f702120011)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
